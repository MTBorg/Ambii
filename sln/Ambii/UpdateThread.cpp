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
}

/*
	//TODO: Comment
*/
VOID UpdateThread::Run() {
	m_stopped = FALSE;

	RECT clientRect;
	GetClientRect(m_hWnd, &clientRect);

	HDC hdcWnd = GetDC(m_hWnd);
	HDC hdcMem = CreateCompatibleDC(hdcWnd);
	HBITMAP hBmpMem = CreateCompatibleBitmap(hdcWnd, clientRect.right, clientRect.bottom);
	SelectObject(hdcMem, hBmpMem);

	m_monitorThreads.clear();
	m_monitorThreads.reserve(m_rSettings.m_usedMonitors.size());
	UINT nLeds = 0; //Count the total amount of output values
	for (CONST auto& monitor : m_rSettings.m_usedMonitors) {
		nLeds += monitor.GetLeftLeds() + monitor.GetRightLeds() + monitor.GetTopLeds() + monitor.GetBottomLeds();
		m_monitorThreads.push_back(MonitorThread(monitor, m_hWnd, m_rSettings, hdcMem));
	}

	m_outputValues = std::make_unique<RGBQUAD[]>(nLeds);

	std::unique_ptr<HANDLE[]> monitorThreadHandles = std::make_unique<HANDLE[]>(m_monitorThreads.size());

	while (TRUE) {
		WaitForSingleObject(m_hMutexSettings, INFINITE);

		for (UINT i = 0; i < m_monitorThreads.size(); i++) {
			monitorThreadHandles[i] = m_monitorThreads.at(i).Create();
		}

		WaitForMultipleObjects(m_monitorThreads.size(), monitorThreadHandles.get(), TRUE, INFINITE);

		if (m_rSettings.m_bDisplayMonitors || m_rSettings.m_bDisplayOutput) {
			BitBlt(hdcWnd, 0, 0, clientRect.right, clientRect.bottom, hdcMem, 0, 0, SRCCOPY);
		}

		ReleaseMutex(m_hMutexSettings);

		if (m_stopped) {
			break;
		}
	}

	DeleteDC(hdcMem);
	DeleteObject(hBmpMem);
	ReleaseDC(m_hWnd, hdcWnd);
}

/*
	//TODO: Comment
*/
VOID UpdateThread::Stop() {
	//Signal to the update thread to stop
	m_stopped = TRUE;

	//Wait for it to finish
	WaitForSingleObject(m_hMutexSettings, INFINITE);
	ReleaseMutex(m_hMutexSettings);
}