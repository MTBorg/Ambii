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

}

/*
	Runs the update thread that gets the pixels data on displays the screen and caluculates and displays the output.

	@Remark: Note that UpdateSubThreads() has to be called before this function to allocate memory for the sub-threads (monitor threads).
*/
VOID UpdateThread::Run() {
	if (m_hWnd == NULL)
		return;

	HANDLE * hMonitorThreads = new HANDLE[m_rSettings.m_usedMonitors.size()];
	clock_t startTime = clock();


	DOUBLE limit = 1.0 / m_rSettings.m_maxRefreshRate;
	while (!m_bStopped) {
		ReleaseMutex(m_hMutexSettings);

		if (DOUBLE(clock() - startTime) / CLOCKS_PER_SEC >= limit) { //Refresh-limiter
			for (UINT i = 0; i < m_rSettings.m_usedMonitors.size(); i++) {
				hMonitorThreads[i] = m_vpMonitorThreads.at(i)->Create();
			}

			WaitForMultipleObjects(m_rSettings.m_usedMonitors.size(), hMonitorThreads, TRUE, INFINITE); //Wait for the threads to finish

			for (UINT i = 0; i < m_rSettings.m_usedMonitors.size(); i++) {
				CloseHandle(hMonitorThreads[i]);
			}

			startTime = clock();
		}

		WaitForSingleObject(m_hMutexSettings, INFINITE); //Make sure nothing is trying to stop the thread while it's updating
	}

	ReleaseMutex(m_hMutexSettings);

	delete[] hMonitorThreads;
	hMonitorThreads = NULL;
}

/*
	Stops the update thread by waiting for the mutex object and then stopping the thread.

	@Remark: This function runs on the main thread, whereas UpdateThread::Run runs on it own thread.
*/
VOID UpdateThread::Stop() {
	WaitForSingleObject(m_hMutexSettings, INFINITE);
	m_bStopped = TRUE;
	ReleaseMutex(m_hMutexSettings);
}

/*
	Starts the update thread.

	@Remark: This function runs on the main thread, whereas UpdateThread::Run runs on it own thread.
*/
VOID UpdateThread::Start() {
	m_bStopped = FALSE;
	Create();
}