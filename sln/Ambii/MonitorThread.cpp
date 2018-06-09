/*
	MonitorThread.cpp

	Source file for class MonitorThread

	@author: Martin Terneborg
*/

#include "MonitorThread.h"

#include "OutputThread.h"

/*
	//TODO: Comment
*/
MonitorThread::MonitorThread(CONST Monitor &rMonitor, CONST HWND hWndMain, CONST Settings &rSettings)
	: m_rMonitor(rMonitor), m_hWndMain(hWndMain), m_rSettings(rSettings), m_arrPixels(NULL)
{
}

/*
	//TODO: Comment
*/
VOID MonitorThread::Run() {
	m_rMonitor.GetPixels(m_arrPixels);
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
	StretchDIBits(hdcWnd, m_rMonitor.GetPosX() * clientRect.right / m_rSettings.m_usedMonitors.size(), 0,
		clientRect.right / m_rSettings.m_usedMonitors.size(), clientRect.bottom, //Width and height of destination
		0, 0,
		m_rMonitor.GetWidth(), m_rMonitor.GetHeight(), //Width and height of source
		m_arrPixels,
		&bmi, DIB_RGB_COLORS, SRCCOPY); //Draw to the window

	ReleaseDC(m_hWndMain, hdcWnd);
	DeleteDC(hdcMonitor);
}