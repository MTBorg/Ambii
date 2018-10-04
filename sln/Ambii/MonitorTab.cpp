/*
	MonitorTab.cpp

	Source file for class MonitorTab.

	@author: Martin Terneborg.
*/

#include "MonitorTab.h"

#include <CommCtrl.h>

#include "InitCtrls.h"

#define TEXTLINE_HEIGHT 17

#define CTRLTEXT_LEDS_LEFT L"LEDs left:"
#define CTRLTEXT_LEDS_RIGHT L"LEDs right:"
#define CTRLTEXT_LEDS_TOP L"LEDs top:"
#define CTRLTEXT_LEDS_BOTTOM L"LEDs bottom:"
#define CTRLTEXT_POSITION_HORZ L"Horizontal position:"
#define CTRLTEXT_POSITION_VERT L"Vertical position:"
#define CTRLTEXT_POSITION_LEFT L"Left side position:"
#define CTRLTEXT_POSITION_RIGHT L"Right side position:"
#define CTRLTEXT_POSITION_TOP L"Top side position:"
#define CTRLTEXT_POSITION_BOTTOM L"Bottom side position:"
#define CTRLTEXT_CLOCKWISE_LEFT L"Left side clockwise"
#define CTRLTEXT_CLOCKWISE_RIGHT L"Right side clockwise"
#define CTRLTEXT_CLOCKWISE_TOP L"Top side clockwise"
#define	CTRLTEXT_CLOCKWISE_BOTTOM L"Bottom side clockwise"
#define CTRLTEXT_ENABLE_LEFT L"Enable left side"
#define CTRLTEXT_ENABLE_RIGHT L"Enable right side"
#define CTRLTEXT_ENABLE_TOP L"Enable top side"
#define CTRLTEXT_ENABLE_BOTTOM L"Enable bottom side"

#define ENABLE_X 0
#define LED_TEXT_X 130
#define LED_EDIT_X (LED_TEXT_X + 65)
#define LED_EDIT_WIDTH 50
#define POSITION_TEXT_X (LED_EDIT_X + LED_EDIT_WIDTH + 10)
#define POSITION_EDIT_X (POSITION_TEXT_X + 100)
#define POSITION_EDIT_WIDTH 25
#define CLOCKWISE_MARGIN 10
#define CLOCKWISE_X (POSITION_EDIT_X + POSITION_EDIT_WIDTH + CLOCKWISE_MARGIN)

/*
	//TODO: Comment
*/
LRESULT CALLBACK DisplayCtrlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED) {
			MonitorTab::m_CONTROL_ID idClockwise, idLEDS, idPos, idTextLEDS, idTextPos;
			BOOL bChecked;
			switch (LOWORD(wParam)) {
			case MonitorTab::m_CONTROL_ID::ENABLE_LEFT_CHECKBOX:
				bChecked = IsDlgButtonChecked(hWnd, MonitorTab::m_CONTROL_ID::ENABLE_LEFT_CHECKBOX);
				idClockwise = MonitorTab::CLOCKWISE_LEFT;
				idLEDS = MonitorTab::LEDS_LEFT_EDIT;
				idPos = MonitorTab::POSITION_LEFT_EDIT;
				idTextLEDS = MonitorTab::LEDS_LEFT_TEXT;
				idTextPos = MonitorTab::POSITION_LEFT_TEXT;
				break;
			case MonitorTab::m_CONTROL_ID::ENABLE_RIGHT_CHECKBOX:
				bChecked = IsDlgButtonChecked(hWnd, MonitorTab::m_CONTROL_ID::ENABLE_RIGHT_CHECKBOX);
				idClockwise = MonitorTab::CLOCKWISE_RIGHT;
				idLEDS = MonitorTab::LEDS_RIGHT_EDIT;
				idPos = MonitorTab::POSITION_RIGHT_EDIT;
				idTextLEDS = MonitorTab::LEDS_RIGHT_TEXT;
				idTextPos = MonitorTab::POSITION_RIGHT_TEXT;
				break;
			case MonitorTab::m_CONTROL_ID::ENABLE_TOP_CHECKBOX:
				bChecked = IsDlgButtonChecked(hWnd, MonitorTab::m_CONTROL_ID::ENABLE_TOP_CHECKBOX);
				idClockwise = MonitorTab::CLOCKWISE_TOP;
				idLEDS = MonitorTab::LEDS_TOP_EDIT;
				idPos = MonitorTab::POSITION_TOP_EDIT;
				idTextLEDS = MonitorTab::LEDS_TOP_TEXT;
				idTextPos = MonitorTab::POSITION_TOP_TEXT;
				break;
			case MonitorTab::m_CONTROL_ID::ENABLE_BOTTOM_CHECKBOX:
				bChecked = IsDlgButtonChecked(hWnd, MonitorTab::m_CONTROL_ID::ENABLE_BOTTOM_CHECKBOX);
				idClockwise = MonitorTab::CLOCKWISE_BOTTOM;
				idLEDS = MonitorTab::LEDS_BOTTOM_EDIT;
				idPos = MonitorTab::POSITION_BOTTOM_EDIT;
				idTextLEDS = MonitorTab::LEDS_BOTTOM_TEXT;
				idTextPos = MonitorTab::POSITION_BOTTOM_TEXT;
				break;
			default:
				return 0;
			}
			if (bChecked) {
				EnableWindow(GetDlgItem(hWnd, idClockwise), TRUE);
				EnableWindow(GetDlgItem(hWnd, idLEDS), TRUE);
				EnableWindow(GetDlgItem(hWnd, idPos), TRUE);
				EnableWindow(GetDlgItem(hWnd, idTextLEDS), TRUE);
				EnableWindow(GetDlgItem(hWnd, idTextPos), TRUE);
			}
			else {
				EnableWindow(GetDlgItem(hWnd, idClockwise), FALSE);
				EnableWindow(GetDlgItem(hWnd, idLEDS), FALSE);
				EnableWindow(GetDlgItem(hWnd, idPos), FALSE);
				EnableWindow(GetDlgItem(hWnd, idTextLEDS), FALSE);
				EnableWindow(GetDlgItem(hWnd, idTextPos), FALSE);
			}
		}
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

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

	SetWindowLongPtr(m_hDisplayCtrl, GWLP_WNDPROC, (LONG)DisplayCtrlProc); //Set window procedure

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

	//Enables
	//////////////////////////////////////////////////////////////////////////////
	//Left
	HWND hEnableLeft = InitCheckboxCtrl(m_hDisplayCtrl, ENABLE_X,	0 * TEXTLINE_HEIGHT, CTRLTEXT_ENABLE_LEFT,	(HMENU)m_CONTROL_ID::ENABLE_LEFT_CHECKBOX);
	SendMessage(hEnableLeft, BM_SETCHECK, m_monitor->GetEnableLeft() ? BST_CHECKED : BST_UNCHECKED, NULL);

	//Right
	HWND hEnableRight = InitCheckboxCtrl(m_hDisplayCtrl, ENABLE_X,	1 * TEXTLINE_HEIGHT, CTRLTEXT_ENABLE_RIGHT, (HMENU)m_CONTROL_ID::ENABLE_RIGHT_CHECKBOX);
	SendMessage(hEnableRight, BM_SETCHECK, m_monitor->GetEnableRight() ? BST_CHECKED : BST_UNCHECKED, NULL);

	//Top
	HWND hEnableTop = InitCheckboxCtrl(m_hDisplayCtrl, ENABLE_X,	2 * TEXTLINE_HEIGHT, CTRLTEXT_ENABLE_TOP,	(HMENU)m_CONTROL_ID::ENABLE_TOP_CHECKBOX);
	SendMessage(hEnableTop, BM_SETCHECK, m_monitor->GetEnableTop() ? BST_CHECKED : BST_UNCHECKED, NULL);

	//Bottom
	HWND hEnableBottom = InitCheckboxCtrl(m_hDisplayCtrl, ENABLE_X,	3 * TEXTLINE_HEIGHT, CTRLTEXT_ENABLE_BOTTOM,(HMENU)m_CONTROL_ID::ENABLE_BOTTOM_CHECKBOX);
	SendMessage(hEnableBottom, BM_SETCHECK, m_monitor->GetEnableBottom() ? BST_CHECKED : BST_UNCHECKED, NULL);
	////////////////////////////////////////////////////////////////////////////

	//LEDs
	//////////////////////////////////////////////////
	//Left
	InitTextCtrl(m_hDisplayCtrl, CTRLTEXT_LEDS_LEFT, LED_TEXT_X, 0 * TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::LEDS_LEFT_TEXT);
	HWND hLedsLeft = InitEditCtrl(m_hDisplayCtrl, LED_EDIT_X, 0 * TEXTLINE_HEIGHT, LED_EDIT_WIDTH, TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::LEDS_LEFT_EDIT);
	SendMessage(hLedsLeft, EM_SETLIMITTEXT, 3, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::LEDS_LEFT_EDIT, m_monitor->GetLeftLeds(), FALSE);

	//Right
	InitTextCtrl(m_hDisplayCtrl, CTRLTEXT_LEDS_RIGHT, LED_TEXT_X, 1 * TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::LEDS_RIGHT_TEXT);
	HWND hLedsRight = InitEditCtrl(m_hDisplayCtrl, LED_EDIT_X, 1 * TEXTLINE_HEIGHT, LED_EDIT_WIDTH, TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::LEDS_RIGHT_EDIT);
	SendMessage(hLedsLeft, EM_SETLIMITTEXT, 3, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::LEDS_RIGHT_EDIT, m_monitor->GetRightLeds(), FALSE);

	//Top
	InitTextCtrl(m_hDisplayCtrl, CTRLTEXT_LEDS_TOP, LED_TEXT_X, 2 * TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::LEDS_TOP_TEXT);
	HWND hLedsTop = InitEditCtrl(m_hDisplayCtrl, LED_EDIT_X, 2 * TEXTLINE_HEIGHT, LED_EDIT_WIDTH, TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::LEDS_TOP_EDIT);
	SendMessage(hLedsLeft, EM_SETLIMITTEXT, 3, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::LEDS_TOP_EDIT, m_monitor->GetTopLeds(), FALSE);

	//Bottom
	InitTextCtrl(m_hDisplayCtrl, CTRLTEXT_LEDS_BOTTOM, LED_TEXT_X, 3 * TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::LEDS_BOTTOM_TEXT);
	HWND hLedsBottom = InitEditCtrl(m_hDisplayCtrl, LED_EDIT_X, 3 * TEXTLINE_HEIGHT, LED_EDIT_WIDTH, TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::LEDS_BOTTOM_EDIT);
	SendMessage(hLedsLeft, EM_SETLIMITTEXT, 3, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::LEDS_BOTTOM_EDIT, m_monitor->GetBottomLeds(), FALSE);
	////////////////////////////////////////////////


	//Positions
	////////////////////////////////////////////
	//Left
	InitTextCtrl(m_hDisplayCtrl, CTRLTEXT_POSITION_LEFT, POSITION_TEXT_X, 0 * TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::POSITION_LEFT_TEXT);
	HWND hPosLeft = InitEditCtrl(m_hDisplayCtrl, POSITION_EDIT_X, 0 * TEXTLINE_HEIGHT, POSITION_EDIT_WIDTH, TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::POSITION_LEFT_EDIT);
	SendMessage(hPosLeft, EM_SETLIMITTEXT, 2, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_LEFT_EDIT, m_monitor->GetPosLeft(), FALSE);

	//Right
	InitTextCtrl(m_hDisplayCtrl, CTRLTEXT_POSITION_RIGHT, POSITION_TEXT_X, 1 * TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::POSITION_RIGHT_TEXT);
	HWND hPosRight = InitEditCtrl(m_hDisplayCtrl, POSITION_EDIT_X, 1 * TEXTLINE_HEIGHT, POSITION_EDIT_WIDTH, TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::POSITION_RIGHT_EDIT);
	SendMessage(hPosRight, EM_SETLIMITTEXT, 2, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_RIGHT_EDIT, m_monitor->GetPosRight(), FALSE);

	//Top
	InitTextCtrl(m_hDisplayCtrl, CTRLTEXT_POSITION_TOP, POSITION_TEXT_X, 2 * TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::POSITION_TOP_TEXT);
	HWND hPosTop = InitEditCtrl(m_hDisplayCtrl, POSITION_EDIT_X, 2 * TEXTLINE_HEIGHT, POSITION_EDIT_WIDTH, TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::POSITION_TOP_EDIT);
	SendMessage(hPosTop, EM_SETLIMITTEXT, 2, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_TOP_EDIT, m_monitor->GetPosTop(), FALSE);

	//Bottom
	InitTextCtrl(m_hDisplayCtrl, CTRLTEXT_POSITION_BOTTOM, POSITION_TEXT_X, 3 * TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::POSITION_BOTTOM_TEXT);
	HWND hPosBottom = InitEditCtrl(m_hDisplayCtrl, POSITION_EDIT_X, 3 * TEXTLINE_HEIGHT, POSITION_EDIT_WIDTH, TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::POSITION_BOTTOM_EDIT);
	SendMessage(hPosBottom, EM_SETLIMITTEXT, 2, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_BOTTOM_EDIT, m_monitor->GetPosBottom(), FALSE);

	//Horizontal
	InitTextCtrl(m_hDisplayCtrl, CTRLTEXT_POSITION_HORZ, POSITION_TEXT_X, 4 * TEXTLINE_HEIGHT, NULL);
	HWND hPosHorz = InitEditCtrl(m_hDisplayCtrl, POSITION_EDIT_X, 4 * TEXTLINE_HEIGHT, POSITION_EDIT_WIDTH, TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::POSITION_HORZ_EDIT);
	SendMessage(hPosHorz, EM_SETLIMITTEXT, 2, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_HORZ_EDIT, m_monitor->GetPosX(), FALSE);

	//Vertical
	InitTextCtrl(m_hDisplayCtrl, CTRLTEXT_POSITION_VERT, POSITION_TEXT_X, 5 * TEXTLINE_HEIGHT, NULL);
	HWND hPosVert = InitEditCtrl(m_hDisplayCtrl, POSITION_EDIT_X, 5 * TEXTLINE_HEIGHT, POSITION_EDIT_WIDTH, TEXTLINE_HEIGHT, (HMENU)m_CONTROL_ID::POSITION_VERT_EDIT);
	SendMessage(hPosVert, EM_SETLIMITTEXT, 2, NULL);
	SetDlgItemInt(m_hDisplayCtrl, m_CONTROL_ID::POSITION_VERT_EDIT, m_monitor->GetPosY(), FALSE);
	////////////////////////////////////////////////////

	//Clockwise-checkboxes
	////////////////////////////
	HWND hCbClockwiseLeft = InitCheckboxCtrl(m_hDisplayCtrl, CLOCKWISE_X, 0 * TEXTLINE_HEIGHT, CTRLTEXT_CLOCKWISE_LEFT, (HMENU)m_CONTROL_ID::CLOCKWISE_LEFT);
	SendMessage(hCbClockwiseLeft, BM_SETCHECK ,m_monitor->GetClockwiseLeft() ? BST_CHECKED : BST_UNCHECKED, NULL);

	HWND hCbClockwiseRight = InitCheckboxCtrl(m_hDisplayCtrl, CLOCKWISE_X, 1 * TEXTLINE_HEIGHT, CTRLTEXT_CLOCKWISE_RIGHT, (HMENU)m_CONTROL_ID::CLOCKWISE_RIGHT);
	SendMessage(hCbClockwiseRight, BM_SETCHECK, m_monitor->GetClockwiseRight() ? BST_CHECKED : BST_UNCHECKED, NULL);

	HWND hCbClockwiseTop = InitCheckboxCtrl(m_hDisplayCtrl, CLOCKWISE_X, 2 * TEXTLINE_HEIGHT, CTRLTEXT_CLOCKWISE_TOP, (HMENU)m_CONTROL_ID::CLOCKWISE_TOP);
	SendMessage(hCbClockwiseTop, BM_SETCHECK, m_monitor->GetClockwiseTop() ? BST_CHECKED : BST_UNCHECKED, NULL);

	HWND hCbClockwiseBottom = InitCheckboxCtrl(m_hDisplayCtrl, CLOCKWISE_X, 3 * TEXTLINE_HEIGHT, CTRLTEXT_CLOCKWISE_BOTTOM, (HMENU)m_CONTROL_ID::CLOCKWISE_BOTTOM);
	SendMessage(hCbClockwiseBottom, BM_SETCHECK, m_monitor->GetClockwiseBottom() ? BST_CHECKED : BST_UNCHECKED, NULL);
	//////////////////////////////////////////
	
	//Enable/disable controls
	SendMessage(m_hDisplayCtrl, WM_COMMAND, HIWORD(BN_CLICKED) | LOWORD(m_CONTROL_ID::ENABLE_LEFT_CHECKBOX), NULL);
	SendMessage(m_hDisplayCtrl, WM_COMMAND, HIWORD(BN_CLICKED) | LOWORD(m_CONTROL_ID::ENABLE_RIGHT_CHECKBOX), NULL);
	SendMessage(m_hDisplayCtrl, WM_COMMAND, HIWORD(BN_CLICKED) | LOWORD(m_CONTROL_ID::ENABLE_TOP_CHECKBOX), NULL);	
	SendMessage(m_hDisplayCtrl, WM_COMMAND, HIWORD(BN_CLICKED) | LOWORD(m_CONTROL_ID::ENABLE_BOTTOM_CHECKBOX), NULL);	

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
	Applies the settings in the monitor tab
	to the monitor pointer passed to the constructor of the monitor tab.

	@return TRUE if all settings are applied successfully, otherwise FALSE.
*/
BOOL MonitorTab::ApplySettings() {
	Monitor monitor = *m_monitor;
	if (!GetSettings(monitor)) {
		return FALSE;
	}
	else {
		*m_monitor = monitor;
		return TRUE;
	}
}

/*
	Returns the current settings entered into the tab (without applying them).

	@param monitor: A reference to the monitor object to write to save the settings to.

	@return TRUE if all settings were retrieved successfully, otherwise FALSE.

	@remark: This function does not save values not considered settings (i.e. monitor/display device name and monitor handle).
*/
BOOL MonitorTab::GetSettings(Monitor &monitor) {
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

	//TODO: Check for success on these bool values
	DWORD clockwiseLeft = SendMessage(GetDlgItem(m_hDisplayCtrl, m_CONTROL_ID::CLOCKWISE_LEFT), BM_GETCHECK, NULL, NULL);
	DWORD clockwiseRight = SendMessage(GetDlgItem(m_hDisplayCtrl, m_CONTROL_ID::CLOCKWISE_RIGHT), BM_GETCHECK, NULL, NULL);
	DWORD clockwiseTop = SendMessage(GetDlgItem(m_hDisplayCtrl, m_CONTROL_ID::CLOCKWISE_TOP), BM_GETCHECK, NULL, NULL);
	DWORD clockwiseBottom = SendMessage(GetDlgItem(m_hDisplayCtrl, m_CONTROL_ID::CLOCKWISE_BOTTOM), BM_GETCHECK, NULL, NULL);

	DWORD enableLeft = SendMessage(GetDlgItem(m_hDisplayCtrl, m_CONTROL_ID::ENABLE_LEFT_CHECKBOX), BM_GETCHECK, NULL, NULL);
	DWORD enableRight = SendMessage(GetDlgItem(m_hDisplayCtrl, m_CONTROL_ID::ENABLE_RIGHT_CHECKBOX), BM_GETCHECK, NULL, NULL);
	DWORD enableTop = SendMessage(GetDlgItem(m_hDisplayCtrl, m_CONTROL_ID::ENABLE_TOP_CHECKBOX), BM_GETCHECK, NULL, NULL);
	DWORD enableBottom = SendMessage(GetDlgItem(m_hDisplayCtrl, m_CONTROL_ID::ENABLE_BOTTOM_CHECKBOX), BM_GETCHECK, NULL, NULL);

	//Make sure that no position is the same as any other
	BOOL equalCheck = FALSE;
	equalCheck |= ((posLeft == posRight) || (posLeft == posTop) || (posLeft == posBottom)) && (posLeft != 0);
	equalCheck |= ((posRight == posTop) || (posRight == posBottom)) && (posRight != 0);
	equalCheck |= (posTop == posBottom) && (posTop != 0);
	if (equalCheck) {
		return FALSE;
	}

	//If the input is valid
	if (resultSuccess) {
		monitor.SetLeftLeds(nLedsLeft);
		monitor.SetRightLeds(nLedsRight);
		monitor.SetTopLeds(nLedsTop);
		monitor.SetBottomLeds(nLedsBottom);
		monitor.SetPosX(posX);
		monitor.SetPosY(posY);
		monitor.SetPosLeft(posLeft);
		monitor.SetPosRight(posRight);
		monitor.SetPosTop(posTop);
		monitor.SetPosBottom(posBottom);
		monitor.SetClockwiseLeft(clockwiseLeft);
		monitor.SetClockwiseRight(clockwiseRight);
		monitor.SetClockwiseTop(clockwiseTop);
		monitor.SetClockwiseBottom(clockwiseBottom);
		monitor.SetEnableLeft(enableLeft);
		monitor.SetEnableRight(enableRight);
		monitor.SetEnableTop(enableTop);
		monitor.SetEnableBottom(enableBottom);
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