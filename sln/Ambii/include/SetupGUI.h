/*
	//TODO: Comment
*/
#ifndef SETUPGUI_H
#define SETUPGUI_H

#include <Windows.h>
#include <vector>

#include "Monitor.h"
#include "MonitorTab.h"

/*
	//TODO: Comment
*/
class SetupGUI {
private:
	CONST std::vector<Monitor>& m_selectedMonitors;
	VOID DrawMonitor(CONST HDC& hdc, CONST Monitor& monitor, CONST UINT x, CONST UINT y, CONST UINT width, CONST UINT height);
public:
	//Default constructor
	SetupGUI(CONST std::vector<Monitor>& m_selectedMonitors);

	VOID Draw(CONST HWND hWndParent);
	HWND Create(CONST HWND hWndParent, CONST UINT x, CONST UINT y, CONST UINT width, CONST UINT height);
};

#endif