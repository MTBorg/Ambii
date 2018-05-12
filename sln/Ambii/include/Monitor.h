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
	UINT m_posLeft = 0, m_posRight = 0, m_posTop = 0, m_posBottom = 0;
	BOOL m_clockwiseLeft = FALSE, m_clockwiseRight = FALSE, m_clockwiseTop = FALSE, m_clockwiseBottom = FALSE;

	static BOOL CALLBACK GetMonitorProc(HMONITOR hMonitor, HDC hdcMonitor,
		LPRECT lprcMonitor, LPARAM dwData);
public:
	//Default constructor
	Monitor();

	//Set-functions
	inline VOID SetClockwiseLeft(BOOL bClockwise) { m_clockwiseLeft = bClockwise; };
	inline VOID SetClockwiseRight(BOOL bClockwise) { m_clockwiseRight = bClockwise; };
	inline VOID SetClockwiseTop(BOOL bClockwise) { m_clockwiseTop = bClockwise; };
	inline VOID SetClockwiseBottom(BOOL bClockwise) { m_clockwiseBottom = bClockwise; };
	inline VOID SetPosX(UINT8 posX) { m_posX = posX; };
	inline VOID SetPosY(UINT8 posY) { m_posY = posY; };
	inline VOID SetPosLeft(UINT8 posLeft) { m_posLeft = posLeft; };
	inline VOID SetPosRight(UINT8 posRight) { m_posRight = posRight; };
	inline VOID SetPosTop(UINT8 posTop) { m_posTop = posTop; };
	inline VOID SetPosBottom(UINT8 posBottom) { m_posBottom = posBottom; };
	inline VOID SetLeftLeds(UINT nLeds) { m_nLedsLeft = nLeds; };
	inline VOID SetRightLeds(UINT nLeds) { m_nLedsRight = nLeds; };
	inline VOID SetTopLeds(UINT nLeds) { m_nLedsTop = nLeds; };
	inline VOID SetBottomLeds(UINT nLeds) { m_nLedsBottom = nLeds; };
	inline VOID SetDisplayDeviceName(CONST LPCTSTR name) { m_displayDeviceName = name; };
	inline VOID SetMonitorName(CONST LPCTSTR name) { m_monitorName = name; };
	inline VOID SetMonitorHandle(CONST HMONITOR hMonitor) {if (hMonitor != NULL) m_hMonitor = hMonitor;};

	//Get-functions
	inline BOOL GetClockwiseLeft() CONST { return m_clockwiseLeft; };
	inline BOOL GetClockwiseRight() CONST { return m_clockwiseRight; };
	inline BOOL GetClockwiseTop() CONST { return m_clockwiseTop; };
	inline BOOL GetClockwiseBottom() CONST { return m_clockwiseBottom; };
	inline UINT8 GetPosX() CONST { return m_posX; };
	inline UINT8 GetPosY() CONST { return m_posY; };
	inline UINT8 GetPosLeft() CONST { return m_posLeft; };
	inline UINT8 GetPosRight() CONST { return m_posRight; };
	inline UINT8 GetPosTop() CONST { return m_posTop; };
	inline UINT8 GetPosBottom() CONST { return m_posBottom; };
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