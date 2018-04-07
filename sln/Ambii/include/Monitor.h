/*
	Monitor.h

	Header file for class Monitor

	@author: Martin Terneborg.
*/

#ifndef MONITOR_H
#define MONITOR_H

#include <Windows.h>
#include <vector>

/*
	Class representing a monitor.
*/
class Monitor {
private:
	HMONITOR m_hMonitor;
	std::wstring m_displayDeviceName;
	std::wstring m_monitorName;
	UINT8 m_posX = 0, m_posY = 0; //The horizontal and position of the monitor in the position of the setup (left to right and top to bottom)
	UINT m_nLedsLeft = 0, m_nLedsRight = 0;
	UINT m_nLedsTop = 0, m_nLedsBottom = 0;

	static BOOL CALLBACK GetMonitorProc(HMONITOR hMonitor, HDC hdcMonitor,
		LPRECT lprcMonitor, LPARAM dwData);
public:
	//Default constructor
	Monitor();

	//Set-functions
	inline VOID SetPosX(UINT8 posX) { m_posX = posX; };
	inline VOID SetPosY(UINT8 posY) { m_posY = posY; };
	inline VOID SetLeftLeds(UINT nLeds) { m_nLedsLeft = nLeds; };
	inline VOID SetRightLeds(UINT nLeds) { m_nLedsRight = nLeds; };
	inline VOID SetTopLeds(UINT nLeds) { m_nLedsTop = nLeds; };
	inline VOID SetBottomLeds(UINT nLeds) { m_nLedsBottom = nLeds; };
	inline VOID SetDisplayDeviceName(CONST LPCTSTR name) { m_displayDeviceName = name; };
	inline VOID SetMonitorName(CONST LPCTSTR name) { m_monitorName = name; };
	inline VOID SetMonitorHandle(CONST HMONITOR hMonitor) {if (hMonitor != NULL) m_hMonitor = hMonitor;};

	//Get-functions
	inline UINT8 GetPosX() CONST { return m_posX; };
	inline UINT8 GetPosY() CONST { return m_posY; };
	inline UINT GetLeftLeds() CONST { return m_nLedsLeft; };
	inline UINT GetRightLeds() CONST { return m_nLedsRight; };
	inline UINT GetTopLeds() CONST { return m_nLedsTop; };
	inline UINT GetBottomLeds() CONST { return m_nLedsBottom; };
	inline LONG GetWidth() CONST {
		MONITORINFOEX monitorInfo;
		monitorInfo.cbSize = sizeof(MONITORINFOEX);
		GetMonitorInfo(m_hMonitor, &monitorInfo);
		 
		return monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
	};
	inline LONG GetHeight() CONST {
		MONITORINFOEX monitorInfo;
		monitorInfo.cbSize = sizeof(MONITORINFOEX);
		GetMonitorInfo(m_hMonitor, &monitorInfo);

		return monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;
	};
	inline CONST HMONITOR GetMonitorHandle() CONST { return m_hMonitor; };
	inline CONST std::wstring GetDisplayDeviceName() CONST { return m_displayDeviceName; };
	inline CONST std::wstring GetMonitorName() CONST { return m_monitorName; };
	static std::vector<Monitor> GetMonitors();
	BOOL GetPixels(RGBQUAD * CONST pixels) CONST;
};

#endif