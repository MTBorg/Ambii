/*
	//TODO: Comment
*/

#include "SetupGUI.h"

#include <CommCtrl.h>

#define MONITOR_BORDER_WIDTH 4

/*
	TODO: Comment
*/
HWND SetupGUI::Create(CONST HWND hWndParent, CONST UINT x, CONST UINT y, CONST UINT width, CONST UINT height,
	CONST std::vector<Monitor>& selectedMonitors) {
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
VOID SetupGUI::Draw(CONST HWND hWndParent, CONST std::vector<MonitorTab>& monitorTabs) {
	HDC hdc = GetDC(hWndParent);

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

