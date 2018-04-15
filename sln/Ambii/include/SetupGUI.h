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
	static VOID DrawMonitor(CONST HDC& hdc, CONST Monitor& monitor, CONST UINT x, CONST UINT y, CONST UINT width, CONST UINT height);
public:
	static VOID Draw(CONST HWND hWndParent, CONST std::vector<MonitorTab>& monitorTabs);
	static HWND Create(CONST HWND hWndParent, CONST UINT x, CONST UINT y, CONST UINT width, CONST UINT height,
		CONST std::vector<Monitor>& selectedMonitors);
};

#endif