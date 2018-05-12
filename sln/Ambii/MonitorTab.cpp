/*
	MonitorTab.cpp

	Source file for class MonitorTab.

	@author: Martin Terneborg.
*/

#include "MonitorTab.h"

#include <CommCtrl.h>

#define TEXTLINE_HEIGHT 17

#define EDITTEXT_POSITION_LEFT L"Left side position:"
#define EDITTEXT_POSITION_RIGHT L"Right side position:"
#define EDITTEXT_POSITION_TOP L"Top side position:"
#define EDITTEXT_POSITION_BOTTOM L"Bottom side position:"

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

	CONST UINT8 textSize = 110;

	InitLedsLeft(0, 0);
	InitLedsRight(0, 25);
	InitLedsTop(0, 50);
	InitLedsBottom(0, 75);
	InitPosLeft(150, 0);
	InitPosRight(150, 20);
	InitPosTop(150, 40);
	InitPosBottom(150, 60);
	InitPositionHorz(0, 125, textSize, TEXTLINE_HEIGHT);
	InitPositionVert(0, 150, textSize, TEXTLINE_HEIGHT);

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
	Initializes the position text and edit control for the horizontal position setting.

	@param x: The horizontal position.
	@param y: The vertical position.
	@param textHeight: The height of the text and edit control.

	@return TRUE if the controls are successfully created, otherwise FALSE.
*/
BOOL MonitorTab::InitPositionHorz(CONST INT x, CONST INT y, CONST UINT textWidth, CONST UINT textHeight) {
	HWND hText = CreateWindow(
		WC_STATIC, L"Horizontal position: ",
		WS_CHILD | WS_VISIBLE | SS_SIMPLE,
		x, y,
		textWidth, textHeight,
		m_hDisplayCtrl,
		NULL,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hText, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);

	HWND hEdit = CreateWindow(
		WC_EDIT, L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
		x + textWidth + 5, y,
		25, textHeight,
		m_hDisplayCtrl,
		(HMENU)m_CONTROL_ID::POSITION_HORZ_EDIT,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hEdit, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	SendMessage(hEdit, EM_SETLIMITTEXT, (WPARAM)2, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_HORZ_EDIT, m_monitor->GetPosX(), FALSE);

	return (hEdit != NULL) && (hText != NULL); //Return TRUE if both controls were created successfully
}

/*
	Initializes the position text and edit control for the vertical position setting.

	@param x: The horizontal position.
	@param y: The vertical position.
	@param textHeight: The height of the text and edit control.

	@return TRUE if the controls are successfully created, otherwise FALSE.
*/
BOOL MonitorTab::InitPositionVert(CONST INT x, CONST INT y, CONST UINT textWidth, CONST UINT textHeight) {
	HWND hText = CreateWindow(
		WC_STATIC, L"Vertical position: ",
		WS_CHILD | WS_VISIBLE | SS_SIMPLE,
		x, y,
		textWidth, textHeight,
		m_hDisplayCtrl,
		NULL,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hText, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);

	HWND hEdit = CreateWindow(
		WC_EDIT, L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
		x + textWidth + 5, y,
		25, textHeight,
		m_hDisplayCtrl,
		(HMENU)m_CONTROL_ID::POSITION_VERT_EDIT,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hEdit, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	SendMessage(hEdit, EM_SETLIMITTEXT, (WPARAM)2, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_VERT_EDIT, m_monitor->GetPosY(), FALSE);

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

	UINT posX = GetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_HORZ_EDIT, &getSuccess, FALSE);
	resultSuccess &= getSuccess;

	UINT posY = GetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_VERT_EDIT, &getSuccess, FALSE);
	resultSuccess &= getSuccess;

	UINT posLeft = GetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_LEFT_EDIT, &getSuccess, FALSE);
	resultSuccess &= getSuccess;

	UINT posRight = GetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_RIGHT_EDIT, &getSuccess, FALSE);
	resultSuccess &= getSuccess;

	UINT posTop = GetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_TOP_EDIT, &getSuccess, FALSE);
	resultSuccess &= getSuccess;

	UINT posBottom = GetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_BOTTOM_EDIT, &getSuccess, FALSE);
	resultSuccess &= getSuccess;

	//If the input is valid
	if (resultSuccess) {
		m_monitor->SetLeftLeds(nLedsLeft);
		m_monitor->SetRightLeds(nLedsRight);
		m_monitor->SetTopLeds(nLedsTop);
		m_monitor->SetBottomLeds(nLedsBottom);
		m_monitor->SetPosX(posX);
		m_monitor->SetPosY(posY);
		m_monitor->SetPosLeft(posLeft);
		m_monitor->SetPosRight(posRight);
		m_monitor->SetPosTop(posTop);
		m_monitor->SetPosBottom(posBottom);
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
		SendMessage(GetDlgItem(m_hDisplayCtrl, m_CONTROL_ID::POSITION_HORZ_EDIT), EM_GETMODIFY, NULL, NULL) ||
		SendMessage(GetDlgItem(m_hDisplayCtrl, m_CONTROL_ID::POSITION_VERT_EDIT), EM_GETMODIFY, NULL, NULL) ||
		SendMessage(GetDlgItem(m_hDisplayCtrl, m_CONTROL_ID::POSITION_LEFT_EDIT), EM_GETMODIFY, NULL, NULL) || 
		SendMessage(GetDlgItem(m_hDisplayCtrl, m_CONTROL_ID::POSITION_RIGHT_EDIT), EM_GETMODIFY, NULL, NULL) ||
		SendMessage(GetDlgItem(m_hDisplayCtrl, m_CONTROL_ID::POSITION_TOP_EDIT), EM_GETMODIFY, NULL, NULL) ||
		SendMessage(GetDlgItem(m_hDisplayCtrl, m_CONTROL_ID::POSITION_BOTTOM_EDIT), EM_GETMODIFY, NULL, NULL);
}

/*
	//TODO: Comment
*/
BOOL MonitorTab::InitPosLeft(CONST UINT x, CONST UINT y) {
	HDC hdcMem = CreateCompatibleDC(NULL);
	SelectObject(hdcMem, (HBRUSH)GetStockObject(DEFAULT_GUI_FONT));
	SIZE size;
	GetTextExtentPoint32(hdcMem, EDITTEXT_POSITION_LEFT, lstrlen(EDITTEXT_POSITION_LEFT), &size);
	DeleteDC(hdcMem);

	HWND hText = CreateWindow(
		WC_STATIC, EDITTEXT_POSITION_LEFT,
		WS_CHILD | WS_VISIBLE | SS_SIMPLE,
		x, y,
		size.cx, 20,
		m_hDisplayCtrl,
		NULL,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hText, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);

	HWND hEdit = CreateWindow(
		WC_EDIT, L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
		x + size.cx + 5, y,
		50, 20,
		m_hDisplayCtrl,
		(HMENU)m_CONTROL_ID::POSITION_LEFT_EDIT,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hEdit, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	SendMessage(hEdit, EM_SETLIMITTEXT, (WPARAM)2, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_LEFT_EDIT, m_monitor->GetPosLeft(), FALSE);

	return (hEdit != NULL) && (hText != NULL); //Return TRUE if both controls were created successfully
}

/*
	//TODO: Comment
*/
BOOL MonitorTab::InitPosRight(CONST UINT x, CONST UINT y) {
	HDC hdcMem = CreateCompatibleDC(NULL);
	SelectObject(hdcMem, (HBRUSH)GetStockObject(DEFAULT_GUI_FONT));
	SIZE size;
	GetTextExtentPoint32(hdcMem, EDITTEXT_POSITION_RIGHT, lstrlen(EDITTEXT_POSITION_RIGHT), &size);
	DeleteDC(hdcMem);

	HWND hText = CreateWindow(
		WC_STATIC, EDITTEXT_POSITION_RIGHT,
		WS_CHILD | WS_VISIBLE | SS_SIMPLE,
		x, y,
		size.cx, 20,
		m_hDisplayCtrl,
		NULL,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hText, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);

	HWND hEdit = CreateWindow(
		WC_EDIT, L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
		x + size.cx + 5, y,
		50, 20,
		m_hDisplayCtrl,
		(HMENU)m_CONTROL_ID::POSITION_RIGHT_EDIT,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hEdit, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	SendMessage(hEdit, EM_SETLIMITTEXT, (WPARAM)2, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_RIGHT_EDIT, m_monitor->GetPosRight(), FALSE);

	return (hEdit != NULL) && (hText != NULL); //Return TRUE if both controls were created successfully
}

/*
	//TODO: Comment
*/
BOOL MonitorTab::InitPosTop(CONST UINT x, CONST UINT y) {
	HDC hdcMem = CreateCompatibleDC(NULL);
	SelectObject(hdcMem, (HBRUSH)GetStockObject(DEFAULT_GUI_FONT));
	SIZE size;
	GetTextExtentPoint32(hdcMem, EDITTEXT_POSITION_TOP, lstrlen(EDITTEXT_POSITION_TOP), &size);
	DeleteDC(hdcMem);

	HWND hText = CreateWindow(
		WC_STATIC, EDITTEXT_POSITION_TOP,
		WS_CHILD | WS_VISIBLE | SS_SIMPLE,
		x, y,
		size.cx, 20,
		m_hDisplayCtrl,
		NULL,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hText, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);

	HWND hEdit = CreateWindow(
		WC_EDIT, L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
		x + size.cx + 5, y,
		50, 20,
		m_hDisplayCtrl,
		(HMENU)m_CONTROL_ID::POSITION_TOP_EDIT,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hEdit, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	SendMessage(hEdit, EM_SETLIMITTEXT, (WPARAM)2, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_TOP_EDIT, m_monitor->GetPosTop(), FALSE);

	return (hEdit != NULL) && (hText != NULL); //Return TRUE if both controls were created successfully
}

/*
	//TODO: Comment
*/
BOOL MonitorTab::InitPosBottom(CONST UINT x, CONST UINT y) {
	HDC hdcMem = CreateCompatibleDC(NULL);
	SelectObject(hdcMem, (HBRUSH)GetStockObject(DEFAULT_GUI_FONT));
	SIZE size;
	GetTextExtentPoint32(hdcMem, EDITTEXT_POSITION_BOTTOM, lstrlen(EDITTEXT_POSITION_BOTTOM), &size);
	DeleteDC(hdcMem);

	HWND hText = CreateWindow(
		WC_STATIC, EDITTEXT_POSITION_BOTTOM,
		WS_CHILD | WS_VISIBLE | SS_SIMPLE,
		x, y,
		size.cx, 20,
		m_hDisplayCtrl,
		NULL,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hText, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);

	HWND hEdit = CreateWindow(
		WC_EDIT, L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
		x + size.cx + 5, y,
		50, 20,
		m_hDisplayCtrl,
		(HMENU)m_CONTROL_ID::POSITION_BOTTOM_EDIT,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hEdit, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	SendMessage(hEdit, EM_SETLIMITTEXT, (WPARAM)2, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_BOTTOM_EDIT, m_monitor->GetPosBottom(), FALSE);

	return (hEdit != NULL) && (hText != NULL); //Return TRUE if both controls were created successfully
}