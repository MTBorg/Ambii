/*
	MonitorThread.h

	Header file for class MonitorThread.

	@author: Martin Terneborg
*/

#ifndef MONITOR_THREAD_H
#define MONITOR_THREAD_H

#include "Thread.h"

#include <Windows.h>
#include <memory>

#include "Monitor.h"
#include "Settings.h"

/*
	Handles the updates for a monitor.
*/
class MonitorThread : public Thread {
private:
	CONST Monitor &m_rMonitor;
	CONST HWND m_hWnd;
	CONST Settings &m_rSettings;
	CONST HDC m_hdc;
	std::unique_ptr<RGBQUAD[]> m_arrPixels;
	RGBQUAD * CONST m_output;

	VOID Run();
	VOID DisplayMonitor();
	VOID CalculateLedsLeft();
	VOID CalculateLedsRight();
	VOID CalculateLedsTop();
	VOID CalculateLedsBottom();
	VOID DisplayLedsLeft();
	VOID DisplayLedsRight();
	VOID DisplayLedsTop();
	VOID DisplayLedsBottom();
	inline RGBQUAD GetPixelFromArr(CONST UINT x, CONST UINT y, CONST RGBQUAD *CONST arrPixels, CONST UINT width) { return arrPixels[y * width + x]};
public:
	MonitorThread(CONST Monitor &pMonitor, CONST HWND hWndMain, CONST Settings &rSettings, CONST HDC hdc, RGBQUAD * CONST output);
};

#endif