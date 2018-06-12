/*
	MonitorThread.cpp

	Source file for class MonitorThread.

	@author: Martin Terneborg
*/

#include "MonitorThread.h"

#include "OutputThread.h"

/*
	//TODO: Comment
*/
MonitorThread::MonitorThread(CONST Monitor &rMonitor, CONST HWND hWnd, CONST Settings &rSettings, CONST HDC hdc)
	: m_rMonitor(rMonitor), m_hWnd(hWnd), m_rSettings(rSettings), m_hdc(hdc)
{
	m_arrPixels = std::make_unique<RGBQUAD[]>(rMonitor.GetWidth() * rMonitor.GetHeight());
}

/*
	//TODO: Comment
*/
VOID MonitorThread::Run() {
	m_rMonitor.GetPixels(m_arrPixels.get());

	DisplayMonitor();
}

/*
	Displays the monitor associated with the thread to the window.
*/
VOID MonitorThread::DisplayMonitor() {
	HDC hdcMonitor = CreateDC(m_rMonitor.GetDisplayDeviceName().c_str(), NULL, NULL, NULL);

	RECT clientRect;
	GetClientRect(m_hWnd, &clientRect);

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
	SetStretchBltMode(m_hdc, STRETCH_HALFTONE);
	StretchDIBits(m_hdc,
		m_rMonitor.GetPosX() * clientRect.right / m_rSettings.m_usedMonitors.size(), 0, //Horizontal and vertical position of destionation
		clientRect.right / m_rSettings.m_usedMonitors.size(), clientRect.bottom, //Width and height of destination
		0, 0, //Horizontal and vertical position of source
		m_rMonitor.GetWidth(), m_rMonitor.GetHeight(), //Width and height of source
		m_arrPixels.get(),
		&bmi, DIB_RGB_COLORS, SRCCOPY); //Draw to the window

	DeleteDC(hdcMonitor);
}

/*
	//TODO: Comment
*/
VOID MonitorThread::CalculateLedsLeft() {

}

/*
	//TODO: Comment
*/
VOID MonitorThread::CalculateLedsRight() {

}

/*
	//TODO: Comment
*/
VOID MonitorThread::CalculateLedsTop() {

}

/*
	//TODO: Comment
*/
VOID MonitorThread::CalculateLedsBottom() {

}