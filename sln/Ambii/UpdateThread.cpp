/*
	UpdateThread.cpp

	Source file for class UpdateThread.

	@author Martin Terneborg
*/

#include "UpdateThread.h"

#include "SerialHandler.h"

#include <time.h>
#include <map>

/*
	Overloaded constructor.

	@param hWnd: A handle to the main window.
	@param rSettings: A refrence to the settings object for the program.
	@param hMutexSettings: A handle to the mutex object used to synchronize this thread with the main thread.
*/
UpdateThread::UpdateThread(CONST HWND hWnd, CONST Settings &rSettings, CONST HANDLE hMutexSettings)
	: m_hWnd(hWnd), m_rSettings(rSettings), m_hMutexSettings(hMutexSettings), m_stopped(TRUE)
{
}

/*
	Runs (starts) the thread.
*/
VOID UpdateThread::Run() {
	m_stopped = FALSE;

	RECT clientRect;
	GetClientRect(m_hWnd, &clientRect);

	HDC hdcWnd = GetDC(m_hWnd);
	HDC hdcMem = CreateCompatibleDC(hdcWnd);
	HBITMAP hBmpMem = CreateCompatibleBitmap(hdcWnd, clientRect.right, clientRect.bottom);
	SelectObject(hdcMem, hBmpMem);

	std::vector<MonitorThread> monitorThreads;
	monitorThreads.reserve(m_rSettings.m_usedMonitors.size());

	//Find the max horizontal/vertical position to determine the width/height of the memory DCs
	UINT xMax = 0, yMax = 0;
	for (CONST auto& monitor : m_rSettings.m_usedMonitors) {
		xMax = xMax < monitor.GetPosX() ? monitor.GetPosX() : xMax;
		yMax = yMax < monitor.GetPosY() ? monitor.GetPosY() : yMax;
	}

	//Add one for easier calculations and to avoid division by zero
	xMax++;
	yMax++;

	//Figure out the dimensions of the portions of the client window that the threads will draw to
	CONST UINT drawWidth = clientRect.right / xMax;
	CONST UINT drawHeight = clientRect.bottom / yMax;

	//Calculate the total amount of LEDs
	UINT nLeds = 0;
	for (const auto& monitor : m_rSettings.m_usedMonitors) {
		nLeds += 
			(monitor.GetLeftLeds() * monitor.GetEnableLeft()) + 
			(monitor.GetRightLeds() * monitor.GetEnableRight()) + 
			(monitor.GetTopLeds() * monitor.GetEnableTop()) + 
			(monitor.GetBottomLeds() * monitor.GetEnableBottom());
	}

	std::unique_ptr<RGBQUAD[]> outputVals = std::make_unique<RGBQUAD[]>(nLeds);

	//Create a map between monitor side positions and locations in the output value array
	std::map<UINT, RGBQUAD*> outputMap;
	UINT i = 0;
	UINT pos = 0;
	while (i < nLeds) {
		for (const auto& monitor : m_rSettings.m_usedMonitors) {
			if (monitor.GetPosLeft() == pos && monitor.GetEnableLeft()) {
				outputMap[monitor.GetPosLeft()] = &outputVals[i];
				i += monitor.GetLeftLeds();
				pos++;
			}
			if (monitor.GetPosRight() == pos && monitor.GetEnableRight()) {
				outputMap[monitor.GetPosRight()] = &outputVals[i];
				i += monitor.GetRightLeds();
				pos++;
			}
			if (monitor.GetPosTop() == pos && monitor.GetEnableTop()) {
				outputMap[monitor.GetPosTop()] = &outputVals[i];
				i += monitor.GetTopLeds();
				pos++;
			}
			if (monitor.GetPosBottom() == pos && monitor.GetEnableBottom()) {
				outputMap[monitor.GetPosBottom()] = &outputVals[i];
				i += monitor.GetBottomLeds();
				pos++;
			}
		}
	}
	
	for (const auto& monitor : m_rSettings.m_usedMonitors) {
		monitorThreads.push_back(MonitorThread(monitor, m_hWnd, m_rSettings, hdcMem, drawWidth, drawHeight,
			outputMap[monitor.GetPosLeft()], outputMap[monitor.GetPosRight()], outputMap[monitor.GetPosTop()], outputMap[monitor.GetPosBottom()]));
	}

	std::unique_ptr<HANDLE[]> threadHandles = std::make_unique<HANDLE[]>(monitorThreads.size());
	while (TRUE) {
		WaitForSingleObject(m_hMutexSettings, INFINITE);

		for (UINT i = 0; i < monitorThreads.size(); i++) {
			threadHandles[i] = monitorThreads.at(i).Create();
		}

		WaitForMultipleObjects(monitorThreads.size(), threadHandles.get(), TRUE, INFINITE);

		if (m_rSettings.m_bDisplayMonitors || m_rSettings.m_bDisplayOutput) {
			BitBlt(hdcWnd, 0, 0, clientRect.right, clientRect.bottom, hdcMem, 0, 0, SRCCOPY);
		}

		//SerialHandler::WriteToPort((BYTE*)outputVals.get(), nLeds * 4, m_rSettings.m_portNum);

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
	Stop the update thread. The calling thread is blocked until the update thread finishes.
*/
VOID UpdateThread::Stop() {
	//Signal to the update thread to stop
	m_stopped = TRUE;

	//Wait for it to finish
	WaitForSingleObject(m_hMutexSettings, INFINITE);
	ReleaseMutex(m_hMutexSettings);
}