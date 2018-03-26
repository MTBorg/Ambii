/*
	PortSettingsWnd.h

	Header file for class PortSettingsWnd.

	@author: Martin Terneborg.
*/
#ifndef PORTSELECT_WND_H
#define PORTSELECT_WND_H

#include <Windows.h>

#include "SerialHandler.h"
#include "Settings.h"

/*
	Class for the port settings window.
*/
class PortSettingsWnd {
private:
	static CONST LPCWSTR m_TITLE;
	enum m_CONTROL_ID {
		PORT_EDIT = 1,
		OK_BUTTON,
		CANCEL_BUTTON
	};

	ATOM Register() CONST;
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	VOID CreateControls(CONST HWND hWndParent, CONST UINT nPortNum) CONST;
public:

	BOOL Create(CONST HWND hWndParent, CONST UINT& rnPortNum) CONST;
};

#endif