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
		POSITION_VERT_EDIT,
		POSITION_LEFT_EDIT,
		POSITION_RIGHT_EDIT,
		POSITION_TOP_EDIT,
		POSITION_BOTTOM_EDIT
	};
	HWND m_hDisplayCtrl;
	Monitor * m_monitor;

	BOOL InitControls();
	BOOL InitLedsLeft(CONST INT x, CONST INT y);
	BOOL InitLedsRight(CONST INT x, CONST INT y);
	BOOL InitLedsTop(CONST INT x, CONST INT y);
	BOOL InitLedsBottom(CONST INT x, CONST INT y);
	BOOL InitPosLeft(CONST UINT x, CONST UINT y);
	BOOL InitPosRight(CONST UINT x, CONST UINT y);
	BOOL InitPosTop(CONST UINT x, CONST UINT y);
	BOOL InitPosBottom(CONST UINT x, CONST UINT y);
	BOOL InitPositionHorz(CONST INT x, CONST INT y, CONST UINT textWidth, CONST UINT textHeight);
	BOOL InitPositionVert(CONST INT x, CONST INT y, CONST UINT textWidth, CONST UINT textHeight);
public:
	VOID Show();
	VOID Hide();
	BOOL Create(CONST HWND hTabCtrl, Monitor * CONST monitor);
	BOOL GetSettings();
	BOOL Modified();
};

#endif