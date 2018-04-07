/*
	MonitorTab.h

	Header file for class MonitorTab

	@author: Martin Terneborg
*/

#ifndef MONITORTAB_H
#define MONITORTAB_H

#include <Windows.h>

#include "Monitor.h"

/*
	Class responsible for handling all the controls of the monitor tabs.
*/
class MonitorTab {
private:
	enum m_CONTROL_ID {
		LEDS_LEFT_EDIT = 1,
		LEDS_RIGHT_EDIT,
		LEDS_TOP_EDIT,
		LEDS_BOTTOM_EDIT,
		POSITION_HORZ_EDIT,
		POSITION_VERT_EDIT
	};
	HWND m_hDisplayCtrl;
	Monitor * m_monitor;

	BOOL InitControls();
	BOOL InitLedsLeft(CONST INT x, CONST INT y);
	BOOL InitLedsRight(CONST INT x, CONST INT y);
	BOOL InitLedsTop(CONST INT x, CONST INT y);
	BOOL InitLedsBottom(CONST INT x, CONST INT y);
	BOOL InitPosition(CONST INT x, CONST INT y);
public:
	VOID Show();
	VOID Hide();
	BOOL Create(CONST HWND hTabCtrl, Monitor * CONST monitor);
	BOOL GetSettings();
	BOOL Modified();
};

#endif