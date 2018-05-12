/*
	//TODO: Comment
*/

#include "SetupGUI.h"

#include <CommCtrl.h>

#define MONITOR_BORDER_WIDTH 4

/*
	Overloaded constructor.

	@param selectedMonitors: A reference to a vector containing all currently selected monitors.
*/
SetupGUI::SetupGUI(CONST std::vector<Monitor>& selectedMonitors)
	: m_selectedMonitors(selectedMonitors) {

}

/*
	//TODO: Comment
*/
VOID SetupGUI::Draw(CONST HWND hWndParent, CONST UINT x, CONST UINT y,
	CONST UINT width, CONST UINT height) {
	HDC hdc = GetDC(hWndParent);
	RECT clientRect;
	GetClientRect(hWndParent, &clientRect);

	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hBmp = CreateCompatibleBitmap(hdc, width, height);
	SelectObject(hdcMem, hBmp);

	//Fill the DC with white colour
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
	RECT memRect = { 0, 0, width, height };
	FillRect(hdcMem, &memRect, hBrush);
	DeleteObject(hBrush);


	//Find out the maximal x and y position of the monitors
	UINT xMax = 0, yMax = 0;
	for (UINT i = 0; i < m_selectedMonitors.size(); i++) {
		xMax = (m_selectedMonitors.at(i).GetPosX() > xMax) ? m_selectedMonitors.at(i).GetPosX() : xMax;
		yMax = (m_selectedMonitors.at(i).GetPosY() > yMax) ? m_selectedMonitors.at(i).GetPosY() : yMax;
	}

	//Add one for easier calculations
	xMax++;
	yMax++;

	UINT monitorWidth = width / xMax;
	UINT monitorHeight = height / yMax;
	for (UINT i = 0; i < m_selectedMonitors.size(); i++) {
		DrawMonitor(hdcMem, m_selectedMonitors.at(i),
			monitorWidth * m_selectedMonitors.at(i).GetPosX(), monitorHeight * m_selectedMonitors.at(i).GetPosY(),
			monitorWidth, monitorHeight);
	}

	BitBlt(hdc, x, y, width, height, hdcMem, 0, 0, SRCCOPY);

	DeleteObject(hBmp);
	DeleteDC(hdcMem);
	ReleaseDC(hWndParent, hdc);
}

/*
	//TODO: Comment
*/
VOID SetupGUI::DrawMonitor(CONST HDC hdc, CONST Monitor& monitor, CONST UINT x, CONST UINT y, CONST UINT width, CONST UINT height) {
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(hdc, hBrush);

	RECT monitorRect;
	monitorRect.left = x;
	monitorRect.right = x + width;
	monitorRect.top = y;
	monitorRect.bottom = y + height;

	//Draw the monitor edges
	Rectangle(hdc, x, y, x + MONITOR_BORDER_WIDTH, y + height); //Left edge
	Rectangle(hdc, x, y, x + width, y + MONITOR_BORDER_WIDTH); //Top edge
	Rectangle(hdc, x, y + height - MONITOR_BORDER_WIDTH, x + width, y + height); //Bottom edge
	Rectangle(hdc, x + width - MONITOR_BORDER_WIDTH, y, x + width, y + height); //Right edge
	
	DrawText(hdc, monitor.GetMonitorName().c_str(), lstrlen(monitor.GetMonitorName().c_str()), &monitorRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
