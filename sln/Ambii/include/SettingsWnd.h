/*
	SettingsWnd.h

	Header file for class SettingsWnd.

	@author: Martin Terneborg.
*/

#include <Windows.h>
#include <vector>

#include "Settings.h"
#include "Monitor.h"
#include "UpdateThread.h"

/*
	Class for the settings window.
*/
class SettingsWnd {
private:
	static CONST LPCTSTR m_TITLE;
	enum m_CONTROL_ID{
		SAMPLESIZE_EDIT = 1,
		REFRESHRATE_EDIT,
		MONITOR_LIST,
		SAVE_BUTTON,
		CANCEL_BUTTON,
		PRIMARYMONITOR_CHECKBOX,
		MULTITHREADING_CHECKBOX,
		BAUDRATE_EDIT
	};

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static ATOM Register();
	static BOOL InitControls(CONST HWND hWndParent, CONST Settings &settings);
	static VOID InitRefreshRate(CONST HWND hWndParent, CONST INT x, CONST INT y, CONST UINT8 refreshRate);
	static VOID InitSampleSize(CONST HWND hWndParent, CONST INT x, CONST INT y, CONST UINT sampleSize);
	static VOID InitMonitorList(CONST HWND hWndParent, CONST INT x, CONST INT y, 
		CONST std::vector<Monitor> &selectedMonitors);
	static VOID InitButtons(CONST HWND hWndParent, CONST INT x, CONST INT y);
	static VOID InitBaudRateEdit(CONST HWND hWndParent, CONST UINT x, CONST UINT y,
		CONST UINT baudRate);
	static VOID InitPrimaryCheckBox(CONST HWND hWndParent, CONST INT x, CONST INT y, CONST BOOL checked);
	static VOID InitMultithreadingCheckBox(CONST HWND hWndParent, CONST INT x, CONST INT y, CONST BOOL checked);
	static VOID InitClockwiseCheckBox(CONST HWND hWndParent, CONST UINT x, CONST UINT y,
		CONST BOOL bChecked);

	static BOOL GetSettings(CONST HWND hWnd, Settings * CONST settings);
public:
	static BOOL Create(CONST HWND hWndParent, CONST Settings &settings);
};