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
		ENABLE_LEFT_CHECKBOX = 1,
		ENABLE_RIGHT_CHECKBOX,
		ENABLE_TOP_CHECKBOX,
		ENABLE_BOTTOM_CHECKBOX,
		LEDS_LEFT_EDIT,
		LEDS_RIGHT_EDIT,
		LEDS_TOP_EDIT,
		LEDS_BOTTOM_EDIT,
		LEDS_LEFT_TEXT,
		LEDS_RIGHT_TEXT,
		LEDS_TOP_TEXT,
		LEDS_BOTTOM_TEXT,
		POSITION_HORZ_EDIT,
		POSITION_VERT_EDIT,
		POSITION_LEFT_EDIT,
		POSITION_RIGHT_EDIT,
		POSITION_TOP_EDIT,
		POSITION_BOTTOM_EDIT,
		POSITION_LEFT_TEXT,
		POSITION_RIGHT_TEXT,
		POSITION_TOP_TEXT,
		POSITION_BOTTOM_TEXT,
		CLOCKWISE_LEFT,
		CLOCKWISE_RIGHT,
		CLOCKWISE_TOP,
		CLOCKWISE_BOTTOM
	};
	HWND m_hDisplayCtrl;
	Monitor * m_monitor;

	BOOL InitControls();
	friend LRESULT CALLBACK DisplayCtrlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	VOID Show();
	VOID Hide();
	BOOL Create(CONST HWND hTabCtrl, Monitor * CONST monitor);
	BOOL ApplySettings();
	BOOL GetSettings(Monitor &monitor);
	BOOL Modified();
	inline CONST Monitor GetMonitor() CONST { return *m_monitor; };
};

#endif