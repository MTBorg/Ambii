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
	TODO: Comment
*/
HWND SetupGUI::Create(CONST HWND hWndParent, CONST UINT x, CONST UINT y, CONST UINT width, CONST UINT height) {
	HWND hStatic = CreateWindow(
		WC_STATIC, L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		x, y,
		width, height,
		hWndParent,
		NULL,
		GetModuleHandle(NULL),
		NULL);

	return hStatic;
}

/*
	//TODO: Comment
*/
VOID SetupGUI::Draw(CONST HWND hWndParent) {
	HDC hdc = GetDC(hWndParent);

	//Find out the maximal x and y position of the monitors
	UINT xMax = 0, yMax = 0;
	for (UINT i = 0; i < m_selectedMonitors.size(); i++) {
		xMax = (m_selectedMonitors.at(i).GetPosX() > xMax) ? m_selectedMonitors.at(i).GetPosX() : xMax;
		yMax = (m_selectedMonitors.at(i).GetPosY() > yMax) ? m_selectedMonitors.at(i).GetPosY() : yMax;
	}

	//Make sure no division by zero occurs
	xMax = ((xMax == 0) ? 1 : xMax);
	yMax = ((yMax == 0) ? 1 : yMax);

	RECT clientRect;
	GetClientRect(hWndParent, &clientRect);
	for (UINT i = 0; i < m_selectedMonitors.size(); i++) {
		DrawMonitor(hdc, m_selectedMonitors.at(i),
			clientRect.right / xMax * m_selectedMonitors.at(i).GetPosX(), 300 + 300 / yMax * m_selectedMonitors.at(i).GetPosY(),
			clientRect.right / xMax, clientRect.bottom / yMax);
	}

	ReleaseDC(hWndParent, hdc);
}

/*
	//TODO: Comment
*/
VOID SetupGUI::DrawMonitor(CONST HDC &hdc, CONST Monitor& monitor, CONST UINT x, CONST UINT y, CONST UINT width, CONST UINT height) {
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(hdc, hBrush);

	//Draw the monitor edges
	Rectangle(hdc, x, y, x + MONITOR_BORDER_WIDTH, y + height); //Left edge
	Rectangle(hdc, x, y, x + width, y + MONITOR_BORDER_WIDTH); //Top edge
	Rectangle(hdc, x, y + height - MONITOR_BORDER_WIDTH, x + width, y + height); //Bottom edge
	Rectangle(hdc, x + width - MONITOR_BORDER_WIDTH, y, x + width, y + height); //Right edge

	DeleteObject(hBrush);
}

