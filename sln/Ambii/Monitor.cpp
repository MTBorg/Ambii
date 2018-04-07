/*
	Monitor.cpp

	Source file for class Monitor

	@author: Martin Terneborg
*/

#include "Monitor.h"

/*
	Default constructor.
*/
Monitor::Monitor()
	: m_displayDeviceName(L""), m_monitorName(L""), m_hMonitor(NULL),
	m_posX(0), m_posY(0), m_nLedsLeft(0), m_nLedsRight(0), m_nLedsTop(0), m_nLedsBottom(0)
{

}

/*
	Gets the pixels on the screen(top-bottom order).

	@param pixels: An array where the pixels will be store.
	@param startLine: The line to start reading from (first line is 0).
	@param nLines: The amount of lines to read.

	@return TRUE if successfull, otherwise false.
*/
BOOL Monitor::GetPixels(RGBQUAD * CONST pixels) CONST {
	if (!pixels) {
		return FALSE;
	}

	CONST LONG monitorWidth = GetWidth();
	CONST LONG monitorHeight = GetHeight();

	HDC hdcMonitor = CreateDC(m_displayDeviceName.c_str(), NULL, NULL, NULL);

	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth = monitorWidth;
	bmi.bmiHeader.biHeight = -monitorHeight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = GetDeviceCaps(hdcMonitor, BITSPIXEL);
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biXPelsPerMeter = GetDeviceCaps(hdcMonitor, HORZRES) * 1000 / GetDeviceCaps(hdcMonitor, HORZSIZE);
	bmi.bmiHeader.biYPelsPerMeter = GetDeviceCaps(hdcMonitor, VERTRES) * 1000 / GetDeviceCaps(hdcMonitor, VERTSIZE);
 
	//Create a device context and bitmap in memory
	HDC hdcMem = CreateCompatibleDC(hdcMonitor);
	HBITMAP hBmpMem = CreateCompatibleBitmap(hdcMonitor, monitorWidth, monitorHeight);
	SelectObject(hdcMem, hBmpMem);

	//Get the screen pixels
	BitBlt(hdcMem, 0, 0, monitorWidth, monitorHeight, hdcMonitor, 0, 0, SRCCOPY);
	GetDIBits(hdcMem, hBmpMem, 0, monitorHeight, pixels, &bmi, DIB_RGB_COLORS);

	//Free memory
	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);
	DeleteDC(hdcMonitor);
  	return TRUE;
}

/*
	Returns a vector contaning all connected monitors

	@return a vector containing all connected monitors
*/
std::vector<Monitor> Monitor::GetMonitors() {
	std::vector<Monitor> monitors;

	EnumDisplayMonitors(NULL, NULL, GetMonitorProc, reinterpret_cast<LPARAM>(&monitors));

	return monitors;
}

/*
	Procedure used to obtain monitors (used by function Monitor::GetMonitors).

	@return TRUE if the enumeration should continue (and there are more monitors to enumerate), FALSE otherwise.
*/
BOOL CALLBACK Monitor::GetMonitorProc(HMONITOR hMonitor, HDC hdcMonitor,
	LPRECT lprcMonitor, LPARAM dwData) {

	std::vector<Monitor> * monitors;
	monitors = reinterpret_cast<std::vector<Monitor>*>(dwData);
	Monitor monitor;

	//Get monitor handle
	monitor.m_hMonitor = hMonitor;

	//Get the display device name
	MONITORINFOEX monitorInfo;
	monitorInfo.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(hMonitor, &monitorInfo);
	monitor.m_displayDeviceName = monitorInfo.szDevice;

	//Get the monitor name
	DISPLAY_DEVICE displayDevice;
	displayDevice.cb = sizeof(DISPLAY_DEVICE);
	EnumDisplayDevices(monitorInfo.szDevice, 0, &displayDevice, NULL);
	monitor.m_monitorName = displayDevice.DeviceString;

	monitors->push_back(monitor);

	return TRUE;
}