/*
	MonitorTab.h

	Header file for class MonitorTab

	@author: Martin Terneborg
*/

#ifndef MONITORTAB_H
#define MONITORTAB_H

#include <Windows.h>

#include "Monitor.h"

/*	Class responsible for handling all the controls of the monitor tabs.
*/
class MonitorTab {
private:
	enum m_CONTROL_ID {
		LEDS_LEFT_EDIT = 1,
		LEDS_RIGHT_EDIT,
		LEDS_TOP_EDIT,
		LEDS_BOTTOM_EDIT,
		POSITION_HORZ_EDIT,
		POSITION_VERT_EDIT,
		POSITION_LEFT_EDIT,
		POSITION_RIGHT_EDIT,
		POSITION_TOP_EDIT,
		POSITION_BOTTOM_EDIT,
		CLOCKWISE_LEFT,
		CLOCKWISE_RIGHT,
		CLOCKWISE_TOP,
		CLOCKWISE_BOTTOM
	};
	HWND m_hDisplayCtrl;
	Monitor * m_monitor;

	BOOL InitControls();
	CONST HWND InitTextCtrl(CONST HWND hWndParent, CONST LPCWSTR text, CONST UINT x, CONST UINT y);
	CONST HWND InitEditCtrl(CONST HWND hWndParent, CONST UINT x, CONST UINT y, CONST UINT width, CONST UINT height, CONST HMENU id);
	CONST HWND MonitorTab::InitCheckboxCtrl(CONST HWND hWndParent, CONST UINT x, CONST UINT y, CONST LPCWSTR text, CONST HMENU id);
public:
	VOID Show();
	VOID Hide();
	BOOL Create(CONST HWND hTabCtrl, Monitor * CONST monitor);
	BOOL GetSettings();
	BOOL Modified();
};

#endif