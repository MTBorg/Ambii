#ifndef SETUPGUI_H
#define SETUPGUI_H

#include <Windows.h>
#include <vector>

#include "Monitor.h"

class SetupGUI {
private:
	static BOOL CreateMonitors(CONST HWND hWndParent, CONST std::vector<Monitor>& selectedMonitors);
public:
	static HWND Create(CONST HWND hWndParent, CONST UINT x, CONST UINT y, CONST UINT width, CONST UINT height,
		CONST std::vector<Monitor>& selectedMonitors);
};

#endif