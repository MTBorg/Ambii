/*
	MonitorTab.cpp

	Source file for class MonitorTab

	@author: Martin Terneborg
*/

#include "MonitorTab.h"

#include <CommCtrl.h>

/*
	Creates the monitor tab.

	@param hTabCtrl: Handle to the tab control.
	@param monitor: The monitor associated with the tab.

	@return TRUE if the monitor tab was successfully created, otherwise FALSE.
*/
BOOL MonitorTab::Create(CONST HWND hTabCtrl, Monitor * CONST monitor) {
	CONST UINT xMargin = 5; 
	CONST UINT yMargin = 5;

	RECT headerRect;
	TabCtrl_GetItemRect(hTabCtrl, 0, &headerRect);

	RECT tabRect;
	GetClientRect(hTabCtrl, &tabRect);

	m_hDisplayCtrl = CreateWindow(
		WC_STATIC, L"",
		WS_CHILD,
		xMargin, yMargin + headerRect.bottom,
		tabRect.right - 2 * xMargin, tabRect.bottom - 2 * yMargin - headerRect.bottom,
		hTabCtrl,
		NULL,
		GetModuleHandle(NULL),
		NULL);

	if (m_hDisplayCtrl == NULL) {
		return FALSE;
	}

	m_monitor = monitor;

	InitControls();

	return TRUE;
}

/*
	Initialzies the controls of the monitor tab.

	@return TRUE if all controls were created successfully, otherwise FALSE.
*/
BOOL MonitorTab::InitControls() {

	InitLedsLeft(0, 0);
	InitLedsRight(0, 25);
	InitLedsTop(0, 50);
	InitLedsBottom(0, 75);
	InitPosition(0, 125);

	return TRUE;
}

/*
	Initializes the "LEDs left: " text and edit control.

	@param x: horizontal position.
	@param y: vertical position.

	@return TRUE if the controls are successfully created, otherwise FALSE.
*/
BOOL MonitorTab::InitLedsLeft(CONST INT x, CONST INT y) {
	CONST UINT8 textWidth = 50;

	HWND hText = CreateWindow(
		WC_STATIC, L"LEDs left: ",
		WS_CHILD | WS_VISIBLE | SS_SIMPLE,
		x, y,
		textWidth, 25,
		m_hDisplayCtrl,
		NULL,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hText, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);

	HWND hEdit = CreateWindow(
		WC_EDIT, L"",
		WS_CHILD | WS_VISIBLE | ES_NUMBER | WS_BORDER,
		x + textWidth + 5, y,
		25, 18,
		m_hDisplayCtrl,
		(HMENU)m_CONTROL_ID::LEDS_LEFT_EDIT,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hEdit, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	SendMessage(hEdit, EM_SETLIMITTEXT, (WPARAM)3, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::LEDS_LEFT_EDIT, m_monitor->GetLeftLeds(), FALSE);

	if (hEdit == NULL) {
		return FALSE;
	}

	return TRUE;
}

/*
	Initializes the "LEDs right" text and edit control.

	@param x: The horizontal position.
	@param y: The vertical position.

	@return TRUE if the controls are successfully created, otherwise FALSE.
*/
BOOL MonitorTab::InitLedsRight(CONST INT x, CONST INT y) {
	CONST UINT8 textWidth = 55;
	HWND hText = CreateWindow(
		WC_STATIC, L"LEDs right: ",
		WS_CHILD | WS_VISIBLE | SS_SIMPLE,
		x, y,
		textWidth, 25,
		m_hDisplayCtrl,
		NULL,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hText, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);

	HWND hEdit = CreateWindow(
		WC_EDIT, L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER ,
		x + textWidth + 5, y,
		25, 18,
		m_hDisplayCtrl,
		(HMENU)m_CONTROL_ID::LEDS_RIGHT_EDIT,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hEdit, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	SendMessage(hEdit, EM_SETLIMITTEXT, (WPARAM)3, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::LEDS_RIGHT_EDIT, m_monitor->GetRightLeds(), FALSE);

	return (hEdit != NULL) && (hText != NULL); //Return TRUE if both controls were created successfully
}

/*
	Initializes the "LEDs top" text and edit control.

	@param x: The horizontal position.
	@param y: The vertical position.

	@return TRUE if the controls are successfully created, otherwise FALSE.
*/
BOOL MonitorTab::InitLedsTop(CONST INT x, CONST INT y) {
	CONST INT textWidth = 55;
	HWND hText = CreateWindow(
		WC_STATIC, L"LEDs top: ",
		WS_CHILD | WS_VISIBLE | SS_SIMPLE,
		x, y,
		textWidth, 25,
		m_hDisplayCtrl,
		NULL,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hText, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);

	HWND hEdit = CreateWindow(
		WC_EDIT, L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
		x + textWidth + 5, y,
		25, 18,
		m_hDisplayCtrl,
		(HMENU)m_CONTROL_ID::LEDS_TOP_EDIT,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hEdit, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	SendMessage(hEdit, EM_SETLIMITTEXT, (WPARAM)3, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::LEDS_TOP_EDIT, m_monitor->GetTopLeds(), FALSE);

	return (hEdit != NULL) && (hText != NULL); //Return TRUE if both controls were created successfully
}

/*
	Initializes the "LEDs bottom" text and edit control.

	@param x: The horizontal position.
	@param y: The vertical position.

	@return TRUE if the controls are successfully created, otherwise FALSE.
*/
BOOL MonitorTab::InitLedsBottom(CONST INT x, CONST INT y) {
	CONST UINT8 textWidth = 70;
	HWND hText = CreateWindow(
		WC_STATIC, L"LEDs bottom: ",
		WS_CHILD | WS_VISIBLE| SS_SIMPLE,
		x, y,
		100, 18,
		m_hDisplayCtrl,
		NULL,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hText, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);

	HWND hEdit = CreateWindow(
		WC_EDIT, L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
		x + textWidth + 5, y,
		25, 18,
		m_hDisplayCtrl,
		(HMENU)m_CONTROL_ID::LEDS_BOTTOM_EDIT,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hEdit, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	SendMessage(hEdit, EM_SETLIMITTEXT, (WPARAM)3, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::LEDS_BOTTOM_EDIT, m_monitor->GetBottomLeds(), FALSE);

	return (hEdit != NULL) && (hText != NULL); //Return TRUE if both controls were created successfully
}

/*
	Initializes the position text and edit control for the position setting.

	@param x: The horizontal position.
	@param y: The vertical position.

	@return TRUE if the controls are successfully created, otherwise FALSE.
*/
BOOL MonitorTab::InitPosition(CONST INT x, CONST INT y) {
	CONST UINT8 textWidth = 110;
	HWND hText = CreateWindow(
		WC_STATIC, L"Position (0 is left-most): ",
		WS_CHILD | WS_VISIBLE | SS_SIMPLE,
		x, y,
		textWidth, 30,
		m_hDisplayCtrl,
		NULL,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hText, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);

	HWND hEdit = CreateWindow(
		WC_EDIT, L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
		x + textWidth + 5, y,
		25, 18,
		m_hDisplayCtrl,
		(HMENU)m_CONTROL_ID::POSITION_EDIT,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hEdit, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	SendMessage(hEdit, EM_SETLIMITTEXT, (WPARAM)2, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_EDIT, m_monitor->GetPosition(), FALSE);

	return (hEdit != NULL) && (hText != NULL); //Return TRUE if both controls were created successfully
}

/*
	Shows all of the tab's controls.
*/
VOID MonitorTab::Show() {
	ShowWindow(m_hDisplayCtrl, SW_SHOW);
}

/*
	Hides all of the tab's controls.
*/
VOID MonitorTab::Hide() {
	ShowWindow(m_hDisplayCtrl, SW_HIDE);
}

/*
	Gets the settings in the monitor tab and copies them (if the input is valid)
	to the monitor reference passed to the constructor of the monitor tab.

	@return TRUE if all settings are retrieved successfully, otherwise FALSE.
*/
BOOL MonitorTab::GetSettings(){
	BOOL resultSuccess = TRUE, getSuccess = TRUE;

	UINT nLedsLeft = GetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::LEDS_LEFT_EDIT, &getSuccess, FALSE);
	resultSuccess &= getSuccess;

	UINT nLedsRight = GetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::LEDS_RIGHT_EDIT, &getSuccess, FALSE);
	resultSuccess &= getSuccess;

	UINT nLedsTop = GetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::LEDS_TOP_EDIT, &getSuccess, FALSE);
	resultSuccess &= getSuccess;

	UINT nLedsBottom = GetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::LEDS_BOTTOM_EDIT, &getSuccess, FALSE);
	resultSuccess &= getSuccess;

	UINT nPosition = GetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_EDIT, &getSuccess, FALSE);
	resultSuccess &= getSuccess;

	//If the input is valid
	if (resultSuccess) {
		m_monitor->SetLeftLeds(nLedsLeft);
		m_monitor->SetRightLeds(nLedsRight);
		m_monitor->SetTopLeds(nLedsTop);
		m_monitor->SetBottomLeds(nLedsBottom);
		m_monitor->SetPosition(nPosition);
		return TRUE;
	}
	else {
		return FALSE;
	}
}

/*
	Checks if any of the controls in the tab has been modified.

	@return TRUE if any of the controls have been modified, otherwise FALSE;
*/
BOOL MonitorTab::Modified() {
	return 
		SendMessage(GetDlgItem(m_hDisplayCtrl, m_CONTROL_ID::LEDS_LEFT_EDIT), EM_GETMODIFY, NULL, NULL) ||
		SendMessage(GetDlgItem(m_hDisplayCtrl, m_CONTROL_ID::LEDS_RIGHT_EDIT), EM_GETMODIFY, NULL, NULL) ||
		SendMessage(GetDlgItem(m_hDisplayCtrl, m_CONTROL_ID::LEDS_TOP_EDIT), EM_GETMODIFY, NULL, NULL) ||
		SendMessage(GetDlgItem(m_hDisplayCtrl, m_CONTROL_ID::LEDS_BOTTOM_EDIT), EM_GETMODIFY, NULL, NULL) ||
		SendMessage(GetDlgItem(m_hDisplayCtrl, m_CONTROL_ID::POSITION_EDIT), EM_GETMODIFY, NULL, NULL);
}