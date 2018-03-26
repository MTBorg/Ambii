/*
	MonitorThread.cpp

	Source file for class MonitorThread

	@author: Martin Terneborg
*/

#include "MonitorThread.h"

#include "OutputThread.h"

/*
	Constructor for the class. Allocates memory for the thread.

	@param rMonitor: A refrence to the monitor the thread is responsible for.
	@param hWndMain: A handle to the main window.
	@param rSettings: A reference to the settings object of the program.
*/
MonitorThread::MonitorThread(CONST Monitor &rMonitor, CONST HWND hWndMain, CONST Settings &rSettings)
	: m_rMonitor(rMonitor), m_hWndMain(hWndMain), m_rSettings(rSettings), m_arrPixels(NULL)
{
	AllocPixels();
}

/*
	Destructor for the class. Deallocates member m_arrPixels.
*/
MonitorThread::~MonitorThread(){
	DeAllocPixels();
}

/*
	Runs the thread which gets the pixels data off the monitor and calculates and displays the output.
*/
VOID MonitorThread::Run() {
	m_rMonitor.GetPixels(m_arrPixels);

	//Create a thread to calculate and display the output
	HANDLE hOutputThread = NULL;
	OutputThread outputThread(m_hWndMain, m_rMonitor, m_arrPixels, m_rSettings);
	hOutputThread = outputThread.Create();

	if (m_rSettings.m_bDisplayMonitors)
		DisplayMonitor();

	//Wait for the thread to finish
	WaitForSingleObject(hOutputThread, INFINITE);

	//Close the handle to the output thread
	CloseHandle(hOutputThread);
}

/*
	Allocates memory for the pixel array.
*/
VOID MonitorThread::AllocPixels() {
	m_arrPixels = new RGBQUAD[m_rMonitor.GetWidth() * m_rMonitor.GetHeight()];
}

/*
	Deallocates the memory for the pixel array.
*/
VOID MonitorThread::DeAllocPixels() {
	delete[] m_arrPixels;
	m_arrPixels = NULL;
}

/*
	Displays the monitor associated with the thread to the window.
*/
VOID MonitorThread::DisplayMonitor() {
	HDC hdcWnd = GetDC(m_hWndMain);
	HDC hdcMonitor = CreateDC(m_rMonitor.GetDisplayDeviceName().c_str(), NULL, NULL, NULL);

	RECT clientRect;
	GetClientRect(m_hWndMain, &clientRect);

	//Set up the bitmapinfo header
	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth = m_rMonitor.GetWidth();
	bmi.bmiHeader.biHeight = -m_rMonitor.GetHeight();
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = GetDeviceCaps(hdcMonitor, BITSPIXEL);
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biXPelsPerMeter = GetDeviceCaps(hdcMonitor, HORZRES) * 1000 / GetDeviceCaps(hdcMonitor, HORZSIZE);
	bmi.bmiHeader.biYPelsPerMeter = GetDeviceCaps(hdcMonitor, VERTRES) * 1000 / GetDeviceCaps(hdcMonitor, VERTSIZE);

	//Display the screen
	SetStretchBltMode(hdcWnd, STRETCH_HALFTONE);
	StretchDIBits(hdcWnd, m_rMonitor.GetPosition() * clientRect.right / m_rSettings.m_usedMonitors.size(), 0,
		clientRect.right / m_rSettings.m_usedMonitors.size(), clientRect.bottom, //Width and height of destination
		0, 0,
		m_rMonitor.GetWidth(), m_rMonitor.GetHeight(), //Width and height of source
		m_arrPixels,
		&bmi, DIB_RGB_COLORS, SRCCOPY); //Draw to the window

	ReleaseDC(m_hWndMain, hdcWnd);
	DeleteDC(hdcMonitor);
}