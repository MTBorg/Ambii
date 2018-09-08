/*
	SetupGUI.h

	Header file for class SetupGUI.

	@author: Martin Terneborg
*/
#ifndef SETUPGUI_H
#define SETUPGUI_H

#include <Windows.h>
#include <vector>

#include "Monitor.h"
#include "MonitorTab.h"

/*
	Class responsible for drawing the setup GUI.
*/
class SetupGUI {
private:
	CONST std::vector<Monitor>& m_selectedMonitors;
	VOID DrawMonitor(CONST HDC hdc, CONST Monitor& monitor, CONST UINT x, CONST UINT y, CONST UINT width, CONST UINT height);
	CONST HBITMAP LoadBMPFromCD(CONST LPCWSTR fileName);
public:
	//Default constructor
	SetupGUI(CONST std::vector<Monitor>& m_selectedMonitors);

	VOID Draw(CONST HWND hWndParent, CONST UINT x, CONST UINT y,
		CONST UINT width, CONST UINT height);
	HWND Create(CONST HWND hWndParent, CONST UINT x, CONST UINT y, CONST UINT width, CONST UINT height);
};

#endif