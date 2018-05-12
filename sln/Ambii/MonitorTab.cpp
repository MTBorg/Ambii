/*
	MonitorTab.cpp

	Source file for class MonitorTab.

	@author: Martin Terneborg.
*/

#include "MonitorTab.h"

#include <CommCtrl.h>

#define TEXTLINE_HEIGHT 17

#define EDITTEXT_LEDS_LEFT L"LEDs left:"
#define EDITTEXT_LEDS_RIGHT L"LEDs right:"
#define EDITTEXT_LEDS_TOP L"LEDs top:"
#define EDITTEXT_LEDS_BOTTOM L"LEDs bottom:"
#define EDITTEXT_POSITION_HORZ L"Horizontal position:"
#define EDITTEXT_POSITION_VERT L"Vertical position:"
#define EDITTEXT_POSITION_LEFT L"Left side position:"
#define EDITTEXT_POSITION_RIGHT L"Right side position:"
#define EDITTEXT_POSITION_TOP L"Top side position:"
#define EDITTEXT_POSITION_BOTTOM L"Bottom side position:"

#define POSITION_TEXT_X 150
#define POSITION_EDIT_X 250
#define POSITION_EDIT_WIDTH 25
#define LED_TEXT_X 0
#define LED_EDIT_X 65
#define LED_EDIT_WIDTH 50

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
	Initializes the controls of the monitor tab.

	@return TRUE if all controls were created successfully, otherwise FALSE.
*/
BOOL MonitorTab::InitControls() {

	//TODO: Implement error checking when returning the handles for the controls

	//LEDs
	//////////////////////////////////////////////////
	//Left
	InitTextCtrl(m_hDisplayCtrl, EDITTEXT_LEDS_LEFT, LED_TEXT_X, 0 * TEXTLINE_HEIGHT);
	HWND hLedsLeft = InitEditCtrl(m_hDisplayCtrl, LED_EDIT_X, 0 * TEXTLINE_HEIGHT, LED_EDIT_WIDTH, TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::LEDS_LEFT_EDIT);
	SendMessage(hLedsLeft, EM_SETLIMITTEXT, 3, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::LEDS_LEFT_EDIT, m_monitor->GetLeftLeds(), FALSE);

	//Right
	InitTextCtrl(m_hDisplayCtrl, EDITTEXT_LEDS_RIGHT, LED_TEXT_X, 1 * TEXTLINE_HEIGHT);
	HWND hLedsRight = InitEditCtrl(m_hDisplayCtrl, LED_EDIT_X, 1 * TEXTLINE_HEIGHT, LED_EDIT_WIDTH, TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::LEDS_RIGHT_EDIT);
	SendMessage(hLedsLeft, EM_SETLIMITTEXT, 3, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::LEDS_RIGHT_EDIT, m_monitor->GetRightLeds(), FALSE);

	//Top
	InitTextCtrl(m_hDisplayCtrl, EDITTEXT_LEDS_TOP, LED_TEXT_X, 2 * TEXTLINE_HEIGHT);
	HWND hLedsTop = InitEditCtrl(m_hDisplayCtrl, LED_EDIT_X, 2 * TEXTLINE_HEIGHT, LED_EDIT_WIDTH, TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::LEDS_TOP_EDIT);
	SendMessage(hLedsLeft, EM_SETLIMITTEXT, 3, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::LEDS_TOP_EDIT, m_monitor->GetTopLeds(), FALSE);

	//Bottom
	InitTextCtrl(m_hDisplayCtrl, EDITTEXT_LEDS_BOTTOM, LED_TEXT_X, 3 * TEXTLINE_HEIGHT);
	HWND hLedsBottom = InitEditCtrl(m_hDisplayCtrl, LED_EDIT_X, 3 * TEXTLINE_HEIGHT, LED_EDIT_WIDTH, TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::LEDS_BOTTOM_EDIT);
	SendMessage(hLedsLeft, EM_SETLIMITTEXT, 3, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::LEDS_BOTTOM_EDIT, m_monitor->GetBottomLeds(), FALSE);
	////////////////////////////////////////////////



	//Positions
	////////////////////////////////////////////
	//Left
	InitTextCtrl(m_hDisplayCtrl, EDITTEXT_POSITION_LEFT, POSITION_TEXT_X, 0 * TEXTLINE_HEIGHT);
	HWND hPosLeft = InitEditCtrl(m_hDisplayCtrl, POSITION_EDIT_X, 0 * TEXTLINE_HEIGHT, POSITION_EDIT_WIDTH, TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::POSITION_LEFT_EDIT);
	SendMessage(hPosLeft, EM_SETLIMITTEXT, 2, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_LEFT_EDIT, m_monitor->GetPosLeft(), FALSE);

	//Right
	InitTextCtrl(m_hDisplayCtrl, EDITTEXT_POSITION_RIGHT, POSITION_TEXT_X, 1 * TEXTLINE_HEIGHT);
	HWND hPosRight = InitEditCtrl(m_hDisplayCtrl, POSITION_EDIT_X, 1 * TEXTLINE_HEIGHT, POSITION_EDIT_WIDTH, TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::POSITION_RIGHT_EDIT);
	SendMessage(hPosRight, EM_SETLIMITTEXT, 2, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_RIGHT_EDIT, m_monitor->GetPosRight(), FALSE);

	//Top
	InitTextCtrl(m_hDisplayCtrl, EDITTEXT_POSITION_TOP, POSITION_TEXT_X, 2 * TEXTLINE_HEIGHT);
	HWND hPosTop = InitEditCtrl(m_hDisplayCtrl, POSITION_EDIT_X, 2 * TEXTLINE_HEIGHT, POSITION_EDIT_WIDTH, TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::POSITION_TOP_EDIT);
	SendMessage(hPosTop, EM_SETLIMITTEXT, 2, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_TOP_EDIT, m_monitor->GetPosTop(), FALSE);

	//Bottom
	InitTextCtrl(m_hDisplayCtrl, EDITTEXT_POSITION_BOTTOM, POSITION_TEXT_X, 3 * TEXTLINE_HEIGHT);
	HWND hPosBottom = InitEditCtrl(m_hDisplayCtrl, POSITION_EDIT_X, 3 * TEXTLINE_HEIGHT, POSITION_EDIT_WIDTH, TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::POSITION_BOTTOM_EDIT);
	SendMessage(hPosBottom, EM_SETLIMITTEXT, 2, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_BOTTOM_EDIT, m_monitor->GetPosBottom(), FALSE);

	//Horizontal
	InitTextCtrl(m_hDisplayCtrl, EDITTEXT_POSITION_HORZ, POSITION_TEXT_X, 4 * TEXTLINE_HEIGHT);
	HWND hPosHorz = InitEditCtrl(m_hDisplayCtrl, POSITION_EDIT_X, 4 * TEXTLINE_HEIGHT, POSITION_EDIT_WIDTH, TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::POSITION_HORZ_EDIT);
	SendMessage(hPosHorz, EM_SETLIMITTEXT, 2, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_HORZ_EDIT, m_monitor->GetPosX(), FALSE);

	//Vertical
	InitTextCtrl(m_hDisplayCtrl, EDITTEXT_POSITION_VERT, POSITION_TEXT_X, 5 * TEXTLINE_HEIGHT);
	HWND hPosVert = InitEditCtrl(m_hDisplayCtrl, POSITION_EDIT_X, 5 * TEXTLINE_HEIGHT, POSITION_EDIT_WIDTH, TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::POSITION_VERT_EDIT);
	SendMessage(hPosVert, EM_SETLIMITTEXT, 2, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_VERT_EDIT, m_monitor->GetPosY(), FALSE);
	////////////////////////////////////////////////////
	return TRUE;
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
BOOL MonitorTab::GetSettings() {
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

	//Make sure that no position is the same as any other
	BOOL equalCheck = equalCheck = ((posLeft == posRight) || (posLeft == posTop) || (posLeft == posBottom)) && (posLeft != 0);
	equalCheck |= ((posRight == posTop) || (posRight == posBottom)) && (posLeft != 0);
	equalCheck |= (posTop == posBottom) && (posTop != 0);
	if (!equalCheck) { //TODO: This should not just return FALSE
		return FALSE;
	}

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
CONST HWND MonitorTab::InitTextCtrl(CONST HWND hWndParent, CONST LPCWSTR text, CONST UINT x, CONST UINT y) {
	//Create a DC in memory to hold the font
	HDC hdcMem = CreateCompatibleDC(NULL);
	SelectObject(hdcMem, (HBRUSH)GetStockObject(DEFAULT_GUI_FONT));
	SIZE size;
	GetTextExtentPoint32(hdcMem, EDITTEXT_POSITION_BOTTOM, lstrlen(EDITTEXT_POSITION_BOTTOM), &size);
	DeleteDC(hdcMem);

	HWND hText = CreateWindow(
		WC_STATIC,
		text,
		WS_CHILD | WS_VISIBLE,
		x, y,
		size.cx, size.cy,
		hWndParent,
		NULL,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hText, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);

	return hText;
}

/*
	//TODO: Comment
*/
CONST HWND MonitorTab::InitEditCtrl(CONST HWND hWndParent, CONST UINT x, CONST UINT y, CONST UINT width, CONST UINT height, CONST HMENU id) {
	HWND hEdit = CreateWindow(
		WC_EDIT, L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
		x, y,
		width, height,
		hWndParent,
		id,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hEdit, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);

	return hEdit;
}