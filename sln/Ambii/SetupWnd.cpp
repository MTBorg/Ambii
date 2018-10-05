/*
	SetupWnd.cpp

	Source file for class SetupWnd.

	@author: Martin Terneborg.
*/

#include "SetupWnd.h"

#include <CommCtrl.h>

#include "InitCtrls.h"

CONST LPCWSTR SetupWnd::m_TITLE = L"Setup";

#define TAB_HEIGHT 300
#define GUI_HEIGHT 500 
#define WIDTH_WND 800

#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 20

#define CONTROLTEXT_SAVEANDEXIT L"Save && exit"
#define CONTROLTEXT_CANCEL L"Cancel"
#define CONTROLTEXT_APPLY L"Apply"

/*
	Overloaded constructor.

	@param selectedMonitors: A reference to the vector containing all currently selected monitors, used to initialize the setup GUI.
*/
SetupWnd::SetupWnd(CONST std::vector<Monitor>& selectedMonitors)
	: m_setupGUI(selectedMonitors)
{

}

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
	HWND hTabCtrl;
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
		case m_CONTROLS_ID::APPLY_BUTTON:
			InvalidateRect(hWnd, NULL, FALSE);
			if (!pObj->ApplySettings(hWnd)) {
				MessageBox(hWnd, L"One or more monitor sides containing LEDs has the same position as some other monitor sides", L"Warning", MB_ICONASTERISK);
			}
			break;
		case m_CONTROLS_ID::SAVE_BUTTON:
			if (!pObj->ApplySettings(hWnd)) {
				MessageBox(hWnd, L"One or more monitor sides containing LEDs has the same position as some other monitor sides", L"Warning", MB_ICONASTERISK);
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
			UpdateWindow(hWnd);
			break;
		case TCN_SELCHANGING:
			return FALSE; //This is necessary in order to be able to switch tab
			break;
		}
		break;
	case WM_PAINT:
		PAINTSTRUCT ps;
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		BeginPaint(hWnd, &ps);
		pObj->m_setupGUI.Draw(hWnd, 0, TAB_HEIGHT, clientRect.right, GUI_HEIGHT);
		EndPaint(hWnd, &ps);
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
		WIDTH_WND, TAB_HEIGHT + GUI_HEIGHT + BUTTON_HEIGHT,
		hWndParent,
		NULL,
		GetModuleHandle(NULL),
		this);

	if (hWnd == NULL) {
		return FALSE;
	}

	//Readjust the window size to account for the caption bar and for all controls
	RECT wndRect, clientRect;
	GetWindowRect(hWnd, &wndRect);
	GetClientRect(hWnd, &clientRect);
	UINT captionHeight = (wndRect.bottom - wndRect.top) - (clientRect.bottom - clientRect.top); //Calculate the height of the caption bar
	SetWindowPos(hWnd, NULL, NULL, NULL, wndRect.right - wndRect.left, wndRect.bottom - wndRect.top + captionHeight, SWP_NOMOVE | SWP_NOZORDER);

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

		//TODO: Check for and handle null handles

		//Initialize buttons
		//Save and exit
		InitBtnCtrl(hWndParent,
			clientRect.right / 2 - 1.5 * BUTTON_WIDTH, TAB_HEIGHT + GUI_HEIGHT,
			BUTTON_WIDTH, BUTTON_HEIGHT,
			CONTROLTEXT_SAVEANDEXIT,
			(HMENU)m_CONTROLS_ID::SAVE_BUTTON);

		//Cancel
		InitBtnCtrl(hWndParent,
			clientRect.right / 2 - BUTTON_WIDTH / 2, TAB_HEIGHT + GUI_HEIGHT,
			BUTTON_WIDTH, BUTTON_HEIGHT,
			CONTROLTEXT_CANCEL,
			(HMENU)m_CONTROLS_ID::CANCEL_BUTTON);

		//Apply
		InitBtnCtrl(hWndParent,
			clientRect.right / 2 + BUTTON_WIDTH / 2, TAB_HEIGHT + GUI_HEIGHT,
			BUTTON_WIDTH, BUTTON_HEIGHT,
			CONTROLTEXT_APPLY,
			(HMENU)m_CONTROLS_ID::APPLY_BUTTON);

	}
	catch (LPCWSTR str){
		throw str;
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
			throw L"Exception in function InitTabCtrl. Exception: Failed to insert tab control item.";
		}
	}

	//Show the first tab initially
	if (selectedMonitors.size() > 0) {
		m_monitorTabs.at(0).Show();
	}
}

/*
	Applies the settings entered.

	@param hWnd: A handle to the window containing the tab control.

	@return True if all settings are applied successfully, otherwise FALSE.
*/
BOOL SetupWnd::ApplySettings(CONST HWND hWnd) {
	HWND hTabCtrl = GetDlgItem(hWnd, m_CONTROLS_ID::TAB_CTRL); //TODO: GetDlgItem returns NULL on release build
	
	std::vector<Monitor> newMonitors;
	newMonitors.reserve(m_monitorTabs.size());

	//Retrieve all the settings from the tabs.
	for (UINT i = 0; i < (UINT)TabCtrl_GetItemCount(hTabCtrl); i++) {
		//Get the tab control item
		TCITEM tcItem = { 0 };
		tcItem.mask = TCIF_PARAM;
		if (!TabCtrl_GetItem(hTabCtrl, i, &tcItem)) {
			throw L"Failed to retrieve tab.";
		}

		MonitorTab *monitorTab = (MonitorTab*)tcItem.lParam;

		Monitor monitor = monitorTab->GetMonitor();
		if (!monitorTab->GetSettings(monitor)) {
			return FALSE;
		}
		else {
			newMonitors.push_back(monitor);
		}

	}

	//Go through all the monitors and make sure that no position value has the same value as some other position value
	//(For the monitor sides this only applies to those who contain LEDs)
	for (UINT i = 0; i < newMonitors.size(); i++) {
		for (UINT j = 0; j < newMonitors.size(); j++) {
			if (i != j) {
				Monitor m1 = newMonitors.at(i), m2 = newMonitors.at(j);
				if (m1.GetEnableLeft()) {
					if ((m1.GetPosLeft() == m2.GetPosLeft()		&& m2.GetEnableLeft())	||
						(m1.GetPosLeft() == m2.GetPosRight()	&& m2.GetEnableRight()) ||
						(m1.GetPosLeft() == m2.GetPosTop()		&& m2.GetEnableTop())	||
						(m1.GetPosLeft() == m2.GetPosBottom()	&& m2.GetEnableBottom())) {
						return FALSE;
					}
				}
				if (m1.GetEnableRight()) {
					if ((m1.GetPosRight() == m2.GetPosLeft()	&& m2.GetEnableLeft())	||
						(m1.GetPosRight() == m2.GetPosRight()	&& m2.GetEnableRight()) ||
						(m1.GetPosRight() == m2.GetPosTop()		&& m2.GetEnableTop())	||
						(m1.GetPosRight() == m2.GetPosBottom()	&& m2.GetEnableBottom())) {
						return FALSE;
					}
				}
				if (m1.GetEnableTop()) {
					if ((m1.GetPosTop() == m2.GetPosLeft()	&& m2.GetEnableLeft())	||
						(m1.GetPosTop() == m2.GetPosRight() && m2.GetEnableRight()) ||
						(m1.GetPosTop() == m2.GetPosTop()	&& m2.GetEnableTop())	||
						(m1.GetPosTop() == m2.GetPosBottom()&& m2.GetEnableBottom())) {
						return FALSE;
					}
				}
				if (m1.GetEnableBottom()) {
					if ((m1.GetPosBottom() == m2.GetPosLeft()	&& m2.GetEnableLeft())	||
						(m1.GetPosBottom() == m2.GetPosRight()	&& m2.GetEnableRight()) ||
						(m1.GetPosBottom() == m2.GetPosTop()	&& m2.GetEnableTop())	||
						(m1.GetPosBottom() == m2.GetPosBottom() && m2.GetEnableBottom())) {
						return FALSE;
					}
				}
			}
		}
	}

	//Apply the settings
	for (UINT i = 0; i < (UINT)TabCtrl_GetItemCount(hTabCtrl); i++) {
		//Get the tab control item
		TCITEM tcItem = { 0 };
		tcItem.mask = TCIF_PARAM;
		if (!TabCtrl_GetItem(hTabCtrl, i, &tcItem)) {
			throw L"Failed to retrieve tab.";
		}

		MonitorTab *monitorTab = (MonitorTab*)tcItem.lParam;
		if (!monitorTab->ApplySettings()) {
			return FALSE;
		}
	}

	return TRUE;
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