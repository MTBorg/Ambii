/*
	UpdateThread.cpp

	Source file for class UpdateThread.

	@author Martin Terneborg
*/

#include "UpdateThread.h"

#include <time.h>

/*
	//TODO: Comment
*/
UpdateThread::UpdateThread(CONST HWND hWnd, CONST Settings &rSettings, CONST HANDLE hMutexSettings)
	: m_hWnd(hWnd), m_rSettings(rSettings), m_hMutexSettings(hMutexSettings), m_stopped(TRUE)
{
	m_monitorThreads.clear();
	m_monitorThreads.reserve(rSettings.m_usedMonitors.size());
	UINT nLeds = 0; //Count the total amount of output values
	for (CONST auto& monitor : rSettings.m_usedMonitors) {
		nLeds += monitor.GetLeftLeds() + monitor.GetRightLeds() + monitor.GetTopLeds() + monitor.GetBottomLeds();
		m_monitorThreads.push_back(MonitorThread(monitor, hWnd, rSettings));
	}
	m_outputValues = std::make_unique<RGBQUAD[]>(nLeds);
}

/*
	//TODO: Comment
*/
VOID UpdateThread::Run() {
	m_stopped = FALSE;

	HDC hdcWnd = GetDC(m_hWnd);
	HDC hdcMem = CreateCompatibleDC(hdcWnd);

	while (1) {
		WaitForSingleObject(m_hMutexSettings, INFINITE);



		ReleaseMutex(m_hMutexSettings);

		if (m_stopped) {
			break;
		}
	}

	DeleteDC(hdcMem);
	ReleaseDC(m_hWnd, hdcWnd);
}

/*
	//TODO: Comment
*/
VOID UpdateThread::Stop() {
	m_stopped = TRUE;
}