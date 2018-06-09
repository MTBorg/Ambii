/*
	UpdateThread.h

	Header file for class UpdateThread

	@author Martin Terneborg
*/
#ifndef UPDATE_THREAD_H
#define UPDATE_THREAD_H

#include "Thread.h"

#include <Windows.h>
#include <vector>
#include <memory>

#include "Settings.h"
#include "MonitorThread.h"

/*
	Class represnting a thread used by the main window to handle updates.
*/
class UpdateThread : public Thread {
private:
	CONST HWND m_hWnd;
	CONST Settings &m_rSettings;
	std::vector<MonitorThread*> m_vpMonitorThreads;
	std::unique_ptr<RGBQUAD> m_output;
	CONST HANDLE m_hMutexSettings;
	BOOL m_bStopped = TRUE;

	VOID Run();
public:
	UpdateThread(CONST HWND hWndMain, CONST Settings &pSettings, CONST HANDLE hMutexSettings);
	VOID UpdateSubThreads();
	
	VOID Stop();
	VOID Start();

	~UpdateThread();

};

#endif