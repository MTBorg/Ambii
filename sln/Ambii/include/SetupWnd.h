/*
	SetupWnd.h

	Header file for class SetupWnd.

	@author: Martin Terneborg.
*/
#ifndef SETUPWND_H
#define SETUPWND_H

#include <Windows.h>
#include <vector>

#include "Monitor.h"
#include "MonitorTab.h"

/*
	Class for the setup window where the user can edit the monitor setup.
*/
class SetupWnd {
private:
	static CONST LPCWSTR m_TITLE;
	enum m_CONTROLS_ID {
		TAB_CTRL = 1,
		SAVE_BUTTON,
		CANCEL_BUTTON
	};
    std::vector<MonitorTab> m_monitorTabs;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg,
		WPARAM lParam, LPARAM wParam);
	ATOM Register();
	VOID InitControls(CONST HWND hWndParent,
		std::vector<Monitor>& selectedMonitors);
	VOID InitTabCtrl(CONST HWND hWndParent,
		std::vector<Monitor>& selectedMonitors, CONST UINT width, CONST UINT height);
	VOID InitButtons(CONST HWND hWndParent, CONST INT x, CONST INT y,
		CONST INT width, CONST INT height);
	VOID InitMonitorWnd(CONST HWND hWndParent, CONST UINT x, CONST UINT y, CONST UINT width, CONST UINT height);
public:
	BOOL Create(CONST HWND hWndParent, 
		std::vector<Monitor>& selectedMonitors);
};

#endif