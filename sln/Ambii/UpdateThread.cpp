/*
	UpdateThread.cpp

	Source file for class UpdateThread.

	@author Martin Terneborg
*/

#include "UpdateThread.h"

#include <time.h>
#include <map>

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

	//Find the max horizontal/vertical position to determine the width/height of the memory DCs
	UINT xMax = 0, yMax = 0;
	for (CONST auto& monitor : m_rSettings.m_usedMonitors) {
		xMax = xMax < monitor.GetPosX() ? monitor.GetPosX() : xMax;
		yMax = yMax < monitor.GetPosY() ? monitor.GetPosY() : yMax;
	}
	xMax++;
	yMax++;
	CONST UINT drawWidth = clientRect.right / xMax;
	CONST UINT drawHeight = clientRect.bottom / yMax;

	std::map<UINT8, RGBQUAD*> outputMap;
	std::vector<std::unique_ptr<RGBQUAD[]>> outputVector;
	outputVector.resize(m_rSettings.m_usedMonitors.size());
	for (UINT i = 0; i < m_rSettings.m_usedMonitors.size(); i++) {
		Monitor monitor = m_rSettings.m_usedMonitors.at(i);
		outputVector.at(i) = std::make_unique<RGBQUAD[]>(monitor.GetLeftLeds() + monitor.GetRightLeds() + monitor.GetTopLeds() + monitor.GetBottomLeds());
		if (monitor.GetLeftLeds() > 0) {
			outputMap[monitor.GetPosLeft()] = &outputVector.at(i)[0];
		}

		if (monitor.GetRightLeds() > 0) {
			outputMap[monitor.GetPosRight()] = &outputVector.at(i)[monitor.GetLeftLeds()];
		}

		if (monitor.GetTopLeds() > 0) {
			outputMap[monitor.GetPosTop()] = &outputVector.at(i)[monitor.GetLeftLeds() + monitor.GetRightLeds()];
		}

		if (monitor.GetBottomLeds() > 0) {
			outputMap[monitor.GetPosBottom()] = &outputVector.at(i)[monitor.GetLeftLeds() + monitor.GetRightLeds() + monitor.GetTopLeds()];
		}

		m_monitorThreads.push_back(MonitorThread(m_rSettings.m_usedMonitors.at(i), m_hWnd, m_rSettings, hdcMem, drawWidth, drawHeight, outputVector.at(i).get()));
	}

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