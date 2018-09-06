/*
	SettingsWnd.cpp

	Source file for class SettingsWnd.

	@author: Martin Terneborg.
*/

#include "SettingsWnd.h"

#include <CommCtrl.h>

#include "InitCtrls.h"

#define CONTROLTEXT_MAXREFRESHRATE L"Max refresh rate:"
#define CONTROLTEXT_SAMPLESIZE L"Sample size:"
#define CONTROLTEXT_SAVEANDEXIT L"Save && exit"
#define CONTROLTEXT_CANCEL L"Cancel"
#define CONTROLTEXT_ONLYPRIMARYMONITOR L"Use only primary monitor"
#define CONTROLTEXT_MULTITHREADING L"Multithreading"
#define CONTROLTEXT_BAUDRATE L"Baud rate:"
#define CONTROLTEXT_CLOCKWISE L"Clockwise"

#define TEXTLIMIT_REFRESHRATE 3
#define TEXTLIMIT_SAMPLESIZE 3
#define TEXTLIMIT_BAUDRATE 6

CONST LPCWSTR SettingsWnd::m_TITLE = L"Settings";

/*
	The window's message procedure.
*/
LRESULT CALLBACK SettingsWnd::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CREATE:
		//Store the pointer to the settings object
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)((LPCREATESTRUCT)lParam)->lpCreateParams);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case m_CONTROL_ID::CANCEL_BUTTON:
			SendMessage(hWnd, WM_CLOSE, NULL, NULL);
			break;
		case m_CONTROL_ID::SAVE_BUTTON: {
			//Retrieve the pointer to the settings object
			Settings * settings = (Settings*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

			Settings oldSettings = *settings;
			if (!GetSettings(hWnd, settings)) {
				MessageBox(hWnd, L"Failed to retrieve settings.", L"Error!", MB_ICONERROR);
				*settings = oldSettings;
			}
			else {
				DestroyWindow(hWnd);
			}
			break;
		}
		}
		break;
	case WM_CLOSE:
		//Check if any of the controls have been modified
		Settings * settings;
		settings = (Settings*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		BOOL bModified;
		bModified = SendMessage(GetDlgItem(hWnd, m_CONTROL_ID::REFRESHRATE_EDIT), EM_GETMODIFY, NULL, NULL) ||
			SendMessage(GetDlgItem(hWnd, m_CONTROL_ID::SAMPLESIZE_EDIT), EM_GETMODIFY, NULL, NULL);

		//If any of the controls have been modified prompt the user with a button asking if he/she wants to
		//discard the changes.
		if(bModified) {
			if (MessageBox(hWnd, L"Settings has been changed, discard changes?", L"Warning",
				MB_ICONASTERISK | MB_OKCANCEL) == IDOK) {
				DestroyWindow(hWnd);
			}
		}
		else {
			DestroyWindow(hWnd);
		}
		break;
	case WM_DESTROY:
		SendMessage(GetWindow(hWnd, GW_OWNER), WM_PARENTNOTIFY, WM_DESTROY, (LPARAM)hWnd); //TODO: This shouldn't be called manually
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}

/*
	Registers the window's class.

	@return The unique class atom of the window class.
*/
ATOM SettingsWnd::Register() {
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

/*
	Gets all the settings from the controls.

	@param hWnd: A handle to the (settings) window.
	@param settings: A pointer to the settings object to be filled with the settings.

	@return TRUE if all settings are successfully retrieved, otherwise FALSE.
*/
BOOL SettingsWnd::GetSettings(CONST HWND hWnd, Settings * CONST settings) {
	//Get the max refresh rate
	BOOL refreshRateSuccess;
	UINT8 refreshRate = GetDlgItemInt(hWnd, m_CONTROL_ID::REFRESHRATE_EDIT,
		&refreshRateSuccess, FALSE);

	//Get the sample size
	BOOL sampleSizeSuccess;
	UINT sampleSize = GetDlgItemInt(hWnd, m_CONTROL_ID::SAMPLESIZE_EDIT,
		&sampleSizeSuccess, FALSE);

	//Get the baud rate
	BOOL baudRateSuccess;
	UINT baudRate = GetDlgItemInt(hWnd, m_CONTROL_ID::BAUDRATE_EDIT,
		&baudRateSuccess, FALSE);

	//Get the status of the checkboxes
	BOOL bPrimaryMonitor = SendMessage(GetDlgItem(hWnd, m_CONTROL_ID::PRIMARYMONITOR_CHECKBOX), BM_GETCHECK,
		NULL, NULL) == BST_CHECKED ? BST_CHECKED : BST_UNCHECKED;
	BOOL bMultiThreading = SendMessage(GetDlgItem(hWnd, m_CONTROL_ID::MULTITHREADING_CHECKBOX), BM_GETCHECK,
		NULL, NULL) == BST_CHECKED ? BST_CHECKED : BST_UNCHECKED;
	HWND hTest = GetDlgItem(hWnd, m_CONTROL_ID::CLOCKWISE_CHECKBOX);
	BOOL bClockwise = SendMessage(GetDlgItem(hWnd, m_CONTROL_ID::CLOCKWISE_CHECKBOX),
		BM_GETCHECK, NULL, NULL);

	//Get the selected monitors
	std::vector<Monitor> allMonitors = Monitor::GetMonitors();
	std::vector<Monitor> selectedMonitors;
	for (INT i = 0; i < ListView_GetItemCount(GetDlgItem(hWnd, m_CONTROL_ID::MONITOR_LIST)); i++) {
		//If item is checked
		if (ListView_GetCheckState(GetDlgItem(hWnd, m_CONTROL_ID::MONITOR_LIST), i)) {
			//Get the item
			LVITEM lvItem;
			lvItem.mask = LVIF_PARAM;
			lvItem.iItem = i;
			ListView_GetItem(GetDlgItem(hWnd, m_CONTROL_ID::MONITOR_LIST), &lvItem);
			HMONITOR hMonitor = (HMONITOR)lvItem.lParam;
		
			//Find the matching monitor by comparing handles
			for (UINT j = 0; j < allMonitors.size(); j++) {
				if (allMonitors.at(j).GetMonitorHandle() == hMonitor) {
					selectedMonitors.push_back(allMonitors.at(j));
				}
			}

		}
	}
	//Copy over all old settings for the monitor if it was previously selected
	for (UINT i = 0; i < selectedMonitors.size(); i++) {
		for (UINT j = 0; j < settings->m_usedMonitors.size(); j++) {
			if (selectedMonitors.at(i).GetMonitorHandle()
				== settings->m_usedMonitors.at(j).GetMonitorHandle()) {
				Monitor oldMonitor = settings->m_usedMonitors.at(j);
				selectedMonitors.at(i).SetLeftLeds(oldMonitor.GetLeftLeds());
				selectedMonitors.at(i).SetRightLeds(oldMonitor.GetRightLeds());
				selectedMonitors.at(i).SetTopLeds(oldMonitor.GetTopLeds());
				selectedMonitors.at(i).SetBottomLeds(oldMonitor.GetBottomLeds());
				selectedMonitors.at(i).SetPosX(oldMonitor.GetPosX());
				selectedMonitors.at(i).SetPosY(oldMonitor.GetPosY());
				break;
			}
		}
	}

	//If every settings was retrieved successfully
	if (sampleSizeSuccess && refreshRateSuccess && baudRateSuccess) {
		settings->m_maxRefreshRate = refreshRate;
		settings->m_sampleSize = sampleSize;
		settings->m_bMultiThreading = bMultiThreading;
		settings->m_bUsePrimaryMonitor = bPrimaryMonitor;
		settings->m_nBaudRate = baudRate;
		settings->m_bClockwise = bClockwise;

		settings->m_usedMonitors.clear();
		settings->m_usedMonitors = selectedMonitors;
		return TRUE;
	}
	else {
		return FALSE;
	}
}

/*
	Creates the window.

	@param hWndParent, A reference to a handle to the parent (main) window
	@param settings, A reference to the settings object that the window will
					save the settings to.

	@return TRUE if the window was created successfully, otherwise FALSE.
*/
BOOL SettingsWnd::Create(CONST HWND hWndParent, CONST Settings &settings) {
	if (hWndParent == NULL) {
		return FALSE;
	}
	Register();

	HWND hWnd = CreateWindow(
		m_TITLE, m_TITLE,
		WS_POPUPWINDOW | WS_VISIBLE | WS_CAPTION,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 300,
		hWndParent,
		NULL,
		GetModuleHandle(NULL),
		(LPVOID)&settings);

	if (hWnd == NULL) {
		return FALSE;
	}

	InitControls(hWnd, settings);

	UpdateWindow(hWnd);
	ShowWindow(hWnd, SW_SHOW);

	return TRUE;
}

/*
	Initializes all the controls of the window.

	@param hWndParent: A handle to the parent (settings) window.
	@param settings: A reference to the settings object containing the settings of the program.

	@return TRUE if successful, otherwise FALSE.
*/
BOOL SettingsWnd::InitControls(CONST HWND hWndParent, CONST Settings &settings) {

	HWND hTxt;
	RECT txtRect;

	//Refresh rate
	hTxt = InitTextCtrl(hWndParent, CONTROLTEXT_MAXREFRESHRATE, 0, 0);
	GetClientRect(hTxt, &txtRect); //Get the text dimensions
	HWND hRefRate = InitEditCtrl(hWndParent, txtRect.right, 0, 20, 20, (HMENU)m_CONTROL_ID::REFRESHRATE_EDIT);
	SendMessage(hRefRate, EM_SETLIMITTEXT, (WPARAM)TEXTLIMIT_REFRESHRATE, NULL);
	SetDlgItemInt(hWndParent, m_CONTROL_ID::REFRESHRATE_EDIT, settings.m_maxRefreshRate, FALSE);

	//Sample size
	hTxt = InitTextCtrl(hWndParent, CONTROLTEXT_SAMPLESIZE, 0, 25);
	GetClientRect(hTxt, &txtRect); //Get the text dimensions
	HWND hSs = InitEditCtrl(hWndParent, txtRect.right, 25, 20, 20, (HMENU)m_CONTROL_ID::SAMPLESIZE_EDIT);
	SendMessage(hSs, EM_SETLIMITTEXT, (WPARAM)TEXTLIMIT_SAMPLESIZE, NULL);
	SetDlgItemInt(hWndParent, m_CONTROL_ID::SAMPLESIZE_EDIT, settings.m_sampleSize, FALSE);

	//Baud rate
	hTxt = InitTextCtrl(hWndParent, CONTROLTEXT_BAUDRATE, 0, 50);
	GetClientRect(hTxt, &txtRect); //Get the text dimensions
	HWND hBaud = InitEditCtrl(hWndParent, txtRect.right, 50, 20, 20, (HMENU)m_CONTROL_ID::BAUDRATE_EDIT);
	SendMessage(hBaud, EM_SETLIMITTEXT, (WPARAM)TEXTLIMIT_BAUDRATE, NULL);
	SetDlgItemInt(hWndParent, m_CONTROL_ID::BAUDRATE_EDIT, settings.m_nBaudRate, FALSE);

	InitMonitorList(hWndParent, 0, 80, settings.m_usedMonitors);
	
	RECT clientRect;
	GetClientRect(hWndParent, &clientRect);

	CONST auto BUTTONWIDTH = 100, BUTTONHEIGHT = 20;
	//Save and exit
	HWND hSaveExit = InitBtnCtrl(hWndParent,
		clientRect.right / 2 - BUTTONWIDTH, clientRect.bottom - BUTTONHEIGHT,
		BUTTONWIDTH, BUTTONHEIGHT,
		CONTROLTEXT_SAVEANDEXIT,
		(HMENU)m_CONTROL_ID::SAVE_BUTTON);

	//Cancel
	HWND hCancel = InitBtnCtrl(hWndParent,
		clientRect.right / 2, clientRect.bottom - BUTTONHEIGHT,
		BUTTONWIDTH, BUTTONHEIGHT,
		CONTROLTEXT_CANCEL,
		(HMENU)m_CONTROL_ID::CANCEL_BUTTON);

	//Only primary monitor
	HWND hPrimaryMonitor = InitCheckboxCtrl(hWndParent, 0, 180, CONTROLTEXT_ONLYPRIMARYMONITOR, (HMENU)m_CONTROL_ID::PRIMARYMONITOR_CHECKBOX);
	SendMessage(hPrimaryMonitor, BM_SETCHECK, (WPARAM)settings.m_bUsePrimaryMonitor ? TRUE : FALSE, NULL);

	//Multithreading
	HWND hMultithreading = InitCheckboxCtrl(hWndParent, 0, 200, CONTROLTEXT_MULTITHREADING, (HMENU)m_CONTROL_ID::MULTITHREADING_CHECKBOX);
	SendMessage(hMultithreading, BM_SETCHECK, (WPARAM)settings.m_bMultiThreading ? TRUE : FALSE, NULL);

	//Clockwise
	HWND hClockwise = InitCheckboxCtrl(hWndParent, 0, 220, CONTROLTEXT_CLOCKWISE, (HMENU)m_CONTROL_ID::CLOCKWISE_CHECKBOX);
	SendMessage(hClockwise, BM_SETCHECK, (WPARAM)settings.m_bClockwise ? TRUE : FALSE, NULL);

	return TRUE;
}

/*
	Initializes the monitor list and text control.

	@param hWndParent: A handle to the parent (settings) window.
	@param x: The horizontal position of the monitor list.
	@param y: The vertical position of the monitor list.
	@param selectedMonitors: A vector containing all currently selected monitors. 
*/
VOID SettingsWnd::InitMonitorList(CONST HWND hWndParent, CONST INT x, CONST INT y,
	CONST std::vector<Monitor> &selectedMonitors) {
	HWND hList = CreateWindow(
		WC_LISTVIEW, L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_EDITLABELS,
		x, y,
		200, 100,
		hWndParent,
		(HMENU)m_CONTROL_ID::MONITOR_LIST,
		GetModuleHandle(NULL),
		NULL);

	if (hList == NULL) {
		throw L"Error initialzing ListView-Control: Monitor List!";
	}

	ListView_SetExtendedListViewStyle(hList,
		LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES);

	//Insert the column
	LVCOLUMN lvColumn;
	lvColumn.cx = 200;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.pszText = L"Monitor";
	lvColumn.iSubItem = 0;
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	ListView_InsertColumn(hList, 0, &lvColumn);

	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT | LVIF_PARAM;
	lvItem.iSubItem = 0;
	lvItem.stateMask = 0; //TODO: This line might not be necessary

	std::vector<Monitor> allMonitors = Monitor::GetMonitors();
	for (UINT i = 0; i < allMonitors.size(); i++) {
		//Add the monitor to the list
		std::wstring monitorName(allMonitors.at(i).GetMonitorName());
		lvItem.pszText = (LPTSTR)monitorName.c_str();
		lvItem.iItem = i;
		lvItem.lParam = (LPARAM)allMonitors.at(i).GetMonitorHandle();
		ListView_InsertItem(hList, &lvItem);

		//Check if monitor is selected
		for (UINT j = 0; j < selectedMonitors.size(); j++) {
			if (selectedMonitors.at(j).GetMonitorHandle() == allMonitors.at(i).GetMonitorHandle()) {
				ListView_SetCheckState(hList, i, TRUE); //Set it as checked
				break;
			}
		}	
	}
}