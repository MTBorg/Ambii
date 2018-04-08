/*
	SetupWnd.cpp

	Source file for class SetupWnd.

	@author: Martin Terneborg.
*/

#include "SetupWnd.h"

#include <CommCtrl.h>

CONST LPCWSTR SetupWnd::m_TITLE = L"Setup";

#define GUI_HEIGHT 300
#define TAB_HEIGHT 300
#define BUTTON_HEIGHT 30
#define BUTTON_WIDTH 100

/*
	The window's message procedure.

	@param hWnd: A handle to the window.
	@param msg: The message to process.
	@param wParam: Word message parameter.
	@param lParam: Long message parameter.

	@return Result of the message processing.
*/
LRESULT CALLBACK SetupWnd::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	SetupWnd * pObj = (SetupWnd*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	switch (msg) {
	case WM_CREATE:
		SetWindowLongPtr(hWnd, GWLP_USERDATA,
			(LONG)(((LPCREATESTRUCT)lParam)->lpCreateParams)); //Store a pointer to the object
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case m_CONTROLS_ID::CANCEL_BUTTON:
			PostMessage(hWnd, WM_CLOSE, NULL, NULL);
			break;
		case m_CONTROLS_ID::SAVE_BUTTON:
			HWND hTabCtrl;
			hTabCtrl = GetDlgItem(hWnd, m_CONTROLS_ID::TAB_CTRL); //TODO: GetDlgItem returns NULL on release build
			for (UINT i = 0; i < (UINT)TabCtrl_GetItemCount(hTabCtrl); i++) {
				//Get the tab control item
				TCITEM tcItem = { 0 };
				tcItem.mask = TCIF_PARAM;
				if (!TabCtrl_GetItem(hTabCtrl, i, &tcItem)) {
					//TODO: Handle error
				}

				MonitorTab *monitorTab = NULL;
				monitorTab = (MonitorTab*)tcItem.lParam;

				if (!monitorTab->GetSettings()) { //TODO: Try/Catch statement
					MessageBox(hWnd, L"Invalid Input!", L"Error", MB_ICONERROR); //TODO: Give better error message
					break;
				}
			}
			DestroyWindow(hWnd);
		break;
		}
		break;
	case WM_NOTIFY:
		switch (((NMHDR*)lParam)->code){
		case TCN_SELCHANGE: //Switch tab
			for (UINT i = 0; i < pObj->m_monitorTabs.size(); i++) {
				pObj->m_monitorTabs.at(i).Hide();
			}
			pObj->m_monitorTabs.at(TabCtrl_GetCurSel(GetDlgItem(hWnd, m_CONTROLS_ID::TAB_CTRL))).Show();
			break;
		case TCN_SELCHANGING:
			return FALSE; //This is necessary in order to be able to switch tab
			break;
		}
		break;
	case WM_CLOSE: 
		BOOL bModified;
		bModified = FALSE;
		//Check if any monitor tab has been modified and prompt the user with a message box asking
		//if they want to discard the monitor if so.
		for (UINT i = 0; i < pObj->m_monitorTabs.size(); i++) {
			bModified = pObj->m_monitorTabs.at(i).Modified();
			if (bModified) {
				if (MessageBox(hWnd, L"Settings has been changed, discard changes?", L"Warning",
					MB_OKCANCEL | MB_ICONASTERISK) == IDOK) {
					DestroyWindow(hWnd);
				}
				break;
			}
		}
		if (!bModified) {
			DestroyWindow(hWnd);
		}
		break;
	case WM_DESTROY:
		SendMessage(GetWindow(hWnd, GW_OWNER), WM_PARENTNOTIFY, WM_DESTROY, (LPARAM)hWnd); //TODO: This shouldn't have to be called manually
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

/*
	Creates the window.

	@param hWndParent: A handle to the parent (main) window.
	@param selectedMonitors: A vector containing all currently selected monitors.

	@return TRUE if the window was created successfully, otherwise FALSE.
*/
BOOL SetupWnd::Create(CONST HWND hWndParent, std::vector<Monitor>& selectedMonitors) {
	Register();
	
	HWND hWnd = CreateWindowEx(
		WS_EX_STATICEDGE,
		m_TITLE, m_TITLE,
		(WS_VISIBLE | WS_OVERLAPPEDWINDOW) ^ (WS_SIZEBOX | WS_MAXIMIZEBOX | WS_MINIMIZEBOX),
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, TAB_HEIGHT + GUI_HEIGHT + BUTTON_WIDTH,
		hWndParent,
		NULL,
		GetModuleHandle(NULL),
		this);

	if (hWnd == NULL) {
		return FALSE;
	}

	InitControls(hWnd, selectedMonitors);

	UpdateWindow(hWnd);
	ShowWindow(hWnd, SW_SHOW);

	return TRUE;
}

/*
	Initializes the controls of the window.

	@param hWndParent: A handle to the parent window of the controls.
	@param selectedMonitors: A reference to the vector containing all the monitors currently selected.

	@throw string received catched from try-block.
*/
VOID SetupWnd::InitControls(CONST HWND hWndParent, std::vector<Monitor>& selectedMonitors) {
	//Initialize the common controls //NOTE: Does not work and does not seem to be necessary
	/*INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icc.dwICC = ICC_TAB_CLASSES;
	InitCommonControlsEx(&icc);*/

	try {
		RECT clientRect;
		GetClientRect(hWndParent, &clientRect);

		InitTabCtrl(hWndParent, selectedMonitors, clientRect.right, TAB_HEIGHT);
		HWND hGUIWnd = SetupGUI::Create(hWndParent, 0, 0 + TAB_HEIGHT, clientRect.right, GUI_HEIGHT, selectedMonitors);

		if (hGUIWnd == NULL) {
			throw L"Exception in function InitControls. Failed to initialize setup GUI (NULL handle).";
		}

		InitButtons(hWndParent, clientRect.right / 2 - BUTTON_WIDTH, clientRect.bottom - BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT);
	}
	catch (LPCWSTR str){
		throw str;
	}
}

/*
	Initializes the "Save & exit" and "Cancel" buttons.

	@param hWndParent: A handle to the parent (setup) window.
	@param x: Horizontal position of the controls.
	@param y: Vertical position of the controls.
	@param width: The width of the buttons.
	@param height: The height of the buttons.

	@throw A string if any of the buttons wasn't successfully created.
*/
VOID SetupWnd::InitButtons(CONST HWND hWndParent, CONST INT x, CONST INT y, CONST INT width, CONST INT height) {
	HWND hSaveButton = CreateWindow(
		WC_BUTTON, L"Save && Exit",
		WS_CHILD | WS_VISIBLE | BS_CENTER,
		x, y,
		width, height,
		hWndParent,
		(HMENU)m_CONTROLS_ID::SAVE_BUTTON,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hSaveButton, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);

	HWND hCancelButton = CreateWindow(
		WC_BUTTON, L"Cancel",
		WS_CHILD | WS_VISIBLE | BS_CENTER,
		x + 100, y,
		width, height,
		hWndParent,
		(HMENU)m_CONTROLS_ID::CANCEL_BUTTON,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hCancelButton, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);

	if (hSaveButton == NULL) {
		throw L"Exception in function InitButtons. Exception: Failed to create button \"Save & Exit\" (NULL handle).";
	}

	if (hCancelButton == NULL) {
		throw L"Exception in function InitButtons. Exception: Failed to create button \"Cancel\" (NULL handle).";
	}
}

/*
	Initializes the tab control.

	@param hWndParent: A handle to the parent (setup) window.
	@param selectedMonitors: A reference to a vector containing all currently selected monitors.
	@param width: The width of the tab control.
	@param height: The height of the tab control.

	@throw String "Failed to create tab control!", if tab control not successfully created
*/
VOID SetupWnd::InitTabCtrl(CONST HWND hWndParent, std::vector<Monitor>& selectedMonitors, CONST UINT width, CONST UINT height) {
	RECT clientRect;
	GetClientRect(hWndParent, &clientRect);
	
	HWND hTabCtrl = CreateWindowEx(
		WS_EX_STATICEDGE,
		WC_TABCONTROL, L"Monitors",
		WS_CHILD | WS_VISIBLE | TCS_FOCUSONBUTTONDOWN,
		0, 0,
		width, height,
		hWndParent,
		(HMENU)m_CONTROLS_ID::TAB_CTRL,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hTabCtrl, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);

	if (hTabCtrl == NULL) {
		throw L"Exception in function InitTabCtrl. Exception: Failed to create tab control (NULL handle).";
	}

	//Create the tabs
	m_monitorTabs = std::vector<MonitorTab>(selectedMonitors.size());
	std::fill(m_monitorTabs.begin(), m_monitorTabs.end(), MonitorTab());
	for (UINT i = 0; i < selectedMonitors.size(); i++) {
		m_monitorTabs.at(i).Create(hTabCtrl, &selectedMonitors.at(i));

		//Create the tab control item
		std::wstring temp(selectedMonitors.at(i).GetMonitorName().c_str());
		TCITEM tcItem = { 0 };
		tcItem.mask = TCIF_TEXT | TCIF_PARAM;
		tcItem.pszText = (LPTSTR)temp.c_str();
		tcItem.cchTextMax = temp.length();
		tcItem.lParam = (LONG)&m_monitorTabs.at(i);

		if (TabCtrl_InsertItem(hTabCtrl, i, &tcItem) == -1) {
			//TODO: Handle error
		}
	}

	//Show the first tab initially
	if (selectedMonitors.size() > 0) {
		m_monitorTabs.at(0).Show();
	}
}

/*
	Registers the window.

	@return The atom of the window.
*/
ATOM SetupWnd::Register() {
	WNDCLASSEX wc;
	wc.cbClsExtra = 0;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = m_TITLE;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	return RegisterClassEx(&wc);
}