/*
	WndMain.cpp

	Source file for class WndMain.

	@author: Martin Terneborg.
*/

#include "WndMain.h"

#include "SettingsWnd.h"
#include "SetupWnd.h"
#include "Monitor.h"
#include "RegistryHandler.h"
#include "UpdateThread.h"
#include "PortSettingsWnd.h"

CONST LPCWSTR WndMain::m_APP_TITLE = L"Ambii";

/*
	Default constructor.
*/
WndMain::WndMain()
	: m_setupWnd(m_settings.m_usedMonitors), m_pUpdateThread(NULL), m_hUpdateThread(NULL)
{

}

/*
	Default destructor.
*/
WndMain::~WndMain() {
	delete m_pUpdateThread;
	m_pUpdateThread = NULL;

	CloseHandle(m_hUpdateThread);
	m_hUpdateThread = NULL;
}

/*
	Runs the main window's message-loop.

	@param hWnd: A handle to the main window.
*/
VOID WndMain::Run(CONST HWND hWnd) {
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

/*
	Message procedure for the main window.

	@param hWnd: A handle to the window.
	@param msg: The message to process.
	@param wParam: Word message parameter.
	@param lParam: Long message parameter.

	@return Result of message procedure.
*/
LRESULT CALLBACK WndMain::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {	
	WndMain * pObj = (WndMain*)GetWindowLongPtr(hWnd, GWLP_USERDATA); //Get the pointer to the object

	static HANDLE hMutexSettings = NULL;

	switch (msg) {
	case WM_CREATE:
		//Store a pointer to the object
		SetWindowLongPtr(hWnd, GWLP_USERDATA, LONG(LPCREATESTRUCT(lParam)->lpCreateParams));
		pObj = (WndMain*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

		//Create a mutex object to protect common data between threads
		hMutexSettings = CreateMutex( NULL, FALSE, NULL);

		pObj->InitMenu(hWnd);	
		try {
			RegistryHandler::GetSettings(pObj->m_settings);

			//If multithreadings is activated create a new thread
			if (pObj->m_settings.m_bMultiThreading) {
				pObj->m_pUpdateThread = new UpdateThread(hWnd, pObj->m_settings, hMutexSettings);
			}
		}
		catch (LPCWSTR errorMsg) {
			MessageBox(hWnd, errorMsg, L"Error!", MB_ICONERROR);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case m_MENU_ITEM::FILE_EXIT:
			SendMessage(hWnd, WM_DESTROY, NULL, NULL);
			break;
		case m_MENU_ITEM::VIEW_MONITORS: {
			UINT prevState = CheckMenuItem(GetMenu(hWnd), m_MENU_ITEM::VIEW_MONITORS, MF_CHECKED); //Get previous state
			pObj->m_settings.m_bDisplayMonitors = prevState == MF_UNCHECKED ? TRUE : FALSE;
			CheckMenuItem(GetMenu(hWnd), m_MENU_ITEM::VIEW_MONITORS, prevState ^ MF_CHECKED); //Invert state
			InvalidateRect(hWnd, NULL, TRUE); //Clear the window
		}
			break;
		case m_MENU_ITEM::VIEW_GUIDE:
			break;
		case m_MENU_ITEM::VIEW_OUTPUT: {
			UINT prevState = CheckMenuItem(GetMenu(hWnd), m_MENU_ITEM::VIEW_OUTPUT, MF_CHECKED); //Get previous state
			pObj->m_settings.m_bDisplayOutput = prevState == MF_UNCHECKED ? TRUE : FALSE;
			CheckMenuItem(GetMenu(hWnd), m_MENU_ITEM::VIEW_OUTPUT, prevState ^ MF_CHECKED); //Invert state
		}
			break;
		case m_MENU_ITEM::EDIT_SETTINGS:
			EnableWindow(hWnd, FALSE);
			if (!SettingsWnd::Create(hWnd, pObj->m_settings)) {
				MessageBox(hWnd, L"Error creating settings window!", L"Error!", MB_ICONERROR);
			}
			break;
		case m_MENU_ITEM::EDIT_SETUP:
			EnableWindow(hWnd, FALSE);
			if (!pObj->m_setupWnd.Create(hWnd, pObj->m_settings.m_usedMonitors)) {
				MessageBox(hWnd, L"Error creating setup window!", L"Error!", MB_ICONERROR);
			}
			break;
		case m_MENU_ITEM::SERIAL_SELECT:
			static PortSettingsWnd PortSettingsWnd;
			PortSettingsWnd.Create(hWnd, pObj->m_settings.m_portNum);
			break;
		case m_MENU_ITEM::SERIAL_RECONNECT:
			SerialHandler::Reconnect();
			break;
		}
		break;
	case WM_PARENTNOTIFY:
		SerialHandler::SetBaudRate(pObj->m_settings.m_portNum, pObj->m_settings.m_nBaudRate); //TODO: This should not be here

		//Bring window to foreground
		EnableWindow(hWnd, TRUE);
		SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		break;
	case WM_DESTROY:
		try {
			RegistryHandler::StoreSettings(pObj->m_settings);
		}
		catch (std::wstring errorMsg) {
			MessageBox(hWnd, errorMsg.c_str(), L"Error!", MB_ICONERROR);
		}

		WaitForSingleObject(pObj->m_hUpdateThread, INFINITE); //Wait for the update thread to finish
		CloseHandle(hMutexSettings);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

/*
	Register the main window.

	@throw //TODO
*/
VOID WndMain::Register() {
	WNDCLASSEX wc;
	wc.cbClsExtra = 0;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WndMain::WndProc;
	wc.lpszClassName = m_APP_TITLE;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) {
		throw L"Exception in function WndMain::Register. Error: Failed to register window class!";
	}
}

/*
	Creates the main window.

	@throw //TODO

	@return A handle to main window.
*/
HWND WndMain::Create() {
	HWND hWnd = CreateWindowEx(
		WS_EX_STATICEDGE,
		m_APP_TITLE,
		m_APP_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		800, 600,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		this);

	if (hWnd == NULL) {
		throw L"Exception in function WndMain::Create. Error: Failed to create main window (NULL handle).";
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return hWnd;
}

/*
	Creates the main window's menu.

	@param hWnd: A handle to the main window.
*/
VOID WndMain::InitMenu(CONST HWND hWnd){
	HMENU hMenu = CreateMenu();

	//File-menu
	HMENU hMenuFile = CreatePopupMenu();
	AppendMenu(hMenuFile, MF_STRING, m_MENU_ITEM::FILE_EXIT, L"Exit");
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hMenuFile, L"File");

	//View-menu
	HMENU hMenuView = CreatePopupMenu();
	AppendMenu(hMenuView, MF_STRING, m_MENU_ITEM::VIEW_MONITORS, L"Monitors");
	AppendMenu(hMenuView, MF_STRING, m_MENU_ITEM::VIEW_OUTPUT, L"Output");
	AppendMenu(hMenuView, MF_POPUP, NULL, NULL);
	AppendMenu(hMenuView, MF_STRING, m_MENU_ITEM::VIEW_GUIDE, L"Guide");
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hMenuView, L"View");

	//Edit-menu
	HMENU hMenuEdit = CreatePopupMenu();
	AppendMenu(hMenuEdit, MF_STRING, m_MENU_ITEM::EDIT_SETTINGS, L"Settings");
	AppendMenu(hMenuEdit, MF_STRING, m_MENU_ITEM::EDIT_SETUP, L"Setup");
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hMenuEdit, L"Edit");

	//Serial-menu
	HMENU hMenuSerial = CreatePopupMenu();
	AppendMenu(hMenuSerial, MF_STRING, m_MENU_ITEM::SERIAL_SELECT, L"Select port");
	AppendMenu(hMenuSerial, MF_STRING, m_MENU_ITEM::SERIAL_RECONNECT, L"Reconnect");
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hMenuSerial, L"Serial");
	

	SetMenu(hWnd, hMenu);
}

/*
	//TODO: Comment
*/
VOID WndMain::UpdateSingleThread(CONST HWND hWnd) {

}