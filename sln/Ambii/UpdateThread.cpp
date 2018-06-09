/*
	UpdateThread.cpp

	Source file for class UpdateThread.

	@author Martin Terneborg
*/

#include "UpdateThread.h"

#include <time.h>

/*
	Constructor for the class. Responsible for intializing values and push the monitors into the member vector.

	@param hWnd: A handle to the window.
	@param rSettings: A reference to the settings object of the program.
	@param hMutexSettings: A handle to a mutex object used to synchronize data between this thread and the main thread.
*/
UpdateThread::UpdateThread(CONST HWND hWnd, CONST Settings &rSettings, CONST HANDLE hMutexSettings)
	: m_hWnd(hWnd), m_rSettings(rSettings), m_hMutexSettings(hMutexSettings)
{
	UINT nLeds = 0;
	for (CONST auto& monitor : rSettings.m_usedMonitors) {
		nLeds += monitor.GetLeftLeds();
		nLeds += monitor.GetRightLeds();
		nLeds += monitor.GetTopLeds();
		nLeds += monitor.GetBottomLeds();
	}
	m_outputValues = std::make_unique<RGBQUAD[]>(nLeds);
}

/*
	Runs the update thread that gets the pixels data on displays the screen and caluculates and displays the output.

	@Remark: Note that UpdateSubThreads() has to be called before this function to allocate memory for the sub-threads (monitor threads).
*/
VOID UpdateThread::Run() {
	
}