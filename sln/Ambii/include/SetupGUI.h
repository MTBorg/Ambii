#ifndef SETUPGUI_H
#define SETUPGUI_H

#include <Windows.h>
#include <vector>

#include "Monitor.h"

class SetupGUI {
private:
	BOOL CreateMonitors(CONST HWND hWndParent, std::vector<Monitor>& selectedMonitors);
public:
	HWND Create(CONST HWND hWndParent, CONST UINT x, CONST UINT y, CONST UINT width, CONST UINT height,
		std::vector<Monitor>& selectedMonitors);
};

#endif