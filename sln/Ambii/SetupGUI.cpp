/*
	//TODO: Comment
*/

#include "SetupGUI.h"

#include <CommCtrl.h>
#include <Strsafe.h>

#define MONITOR_BORDER_WIDTH 4

#define FILENAME_ARROW L"\\Arrow.bmp"
#define FILENAME_LED L"\\LED.bmp"

#define BMP_LED_DISPLAY_WIDTH 20
#define BMP_LED_DISPLAY_HEIGHT 20
#define BMP_ARROW_DISPLAY_HEIGHT 20
#define BMP_ARROW_DISPLAY_WIDTH 20
#define ARROW_MARGIN 20

/*
	Overloaded constructor.

	@param selectedMonitors: A reference to a vector containing all currently selected monitors.
*/
SetupGUI::SetupGUI(CONST std::vector<Monitor>& selectedMonitors)
	: m_selectedMonitors(selectedMonitors) {
}

/*
	Draws the gui.

	@param hWndParent: A handle to the parent window.
	@param x: The horizontal position of the GUI.
	@param y: The vertical position of the GUI.
	@param width: The width of the GUI.
	@parwam height: The height of the GUI.
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

	DeleteDC(hdcMem);
	DeleteObject(hBmp);
	ReleaseDC(hWndParent, hdc);
}

/*
	Draws a monitor in the GUI.

	@param hdc: The hdc object of the GUI.
	@param monitor: A reference to the monitor to be drawn.
	@param x: The horizontal position to draw the monitor at.
	@param y: The vertical position to draw the monitor at.
	@param width: The drawing width of the monitor.
	@param height: The drawing height of the monitor.
*/
VOID SetupGUI::DrawMonitor(CONST HDC hdc, CONST Monitor& monitor, CONST UINT x, CONST UINT y, CONST UINT width, CONST UINT height) {
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(hdc, hBrush);

	//Draw the monitor edges
	Rectangle(hdc, x, y, x + MONITOR_BORDER_WIDTH, y + height); //Left edge
	Rectangle(hdc, x, y, x + width, y + MONITOR_BORDER_WIDTH); //Top edge
	Rectangle(hdc, x, y + height - MONITOR_BORDER_WIDTH, x + width, y + height); //Bottom edge
	Rectangle(hdc, x + width - MONITOR_BORDER_WIDTH, y, x + width, y + height); //Right edge

	HBITMAP hBmpArrow = LoadBMPFromCD(FILENAME_ARROW);
	if (hBmpArrow == NULL) {
		//TODO: Throw exception
	}
	HDC hdcBmpArrow = CreateCompatibleDC(hdc);
	SelectObject(hdcBmpArrow, hBmpArrow);
	BITMAP bmpArrow;
	GetObject(hBmpArrow, sizeof(BITMAP), &bmpArrow);

	HBITMAP hBmpLED = LoadBMPFromCD(FILENAME_LED);
	if (hBmpLED == NULL) {
		//TODO: Throw exception
	}
	HDC hdcBmpLED = CreateCompatibleDC(hdc);
	SelectObject(hdcBmpLED, hBmpLED);
	BITMAP bmpLED;
	GetObject(hBmpLED, sizeof(BITMAP), &bmpLED);

	POINT rotPt[3];

	//LEFT side
	for (UINT i = 0; i < monitor.GetLeftLeds(); i++) {
		StretchBlt(hdc,
			x, i * height / monitor.GetLeftLeds(),
			BMP_LED_DISPLAY_WIDTH, BMP_LED_DISPLAY_HEIGHT,
			hdcBmpLED,
			0, 0,
			bmpLED.bmWidth, bmpLED.bmHeight,
			SRCCOPY);
	}
	if (monitor.GetLeftLeds()) {
		if (monitor.GetClockwiseLeft()) {
			rotPt[0] = { LONG(x + ARROW_MARGIN), LONG(y + height / 2 + BMP_ARROW_DISPLAY_HEIGHT / 2) };
			rotPt[1] = { LONG(x + ARROW_MARGIN), LONG(y + height / 2 - BMP_ARROW_DISPLAY_HEIGHT / 2) };
			rotPt[2] = { LONG(x + ARROW_MARGIN + BMP_ARROW_DISPLAY_HEIGHT), LONG(y + height / 2 + BMP_ARROW_DISPLAY_HEIGHT / 2) };
		}
		else {
			rotPt[0] = { LONG(x + ARROW_MARGIN + BMP_ARROW_DISPLAY_HEIGHT), LONG(y + height / 2 - BMP_ARROW_DISPLAY_HEIGHT / 2) };
			rotPt[1] = { LONG(x + ARROW_MARGIN + BMP_ARROW_DISPLAY_HEIGHT), LONG(y + height / 2 + BMP_ARROW_DISPLAY_HEIGHT / 2) };
			rotPt[2] = { LONG(x + ARROW_MARGIN), LONG(y + height / 2 - BMP_ARROW_DISPLAY_HEIGHT / 2) };
		}

		PlgBlt(hdc, rotPt, hdcBmpArrow, 0, 0, bmpArrow.bmWidth, bmpArrow.bmHeight, NULL, NULL, NULL);
	}

	//RIGHT side
	for (UINT i = 0; i < monitor.GetRightLeds(); i++) {
			StretchBlt(hdc,
				x + width - BMP_LED_DISPLAY_WIDTH, monitor.GetClockwiseRight() ? (y + height - i * height / monitor.GetRightLeds() - BMP_LED_DISPLAY_HEIGHT) : (i * height / monitor.GetRightLeds()),
				BMP_LED_DISPLAY_WIDTH, BMP_LED_DISPLAY_HEIGHT,
				hdcBmpLED,
				0, 0,
				bmpLED.bmWidth, bmpLED.bmHeight,
				SRCCOPY);
	}
	if (monitor.GetRightLeds()) {
		if (monitor.GetClockwiseRight()) {
			rotPt[0] = { LONG(x + width - ARROW_MARGIN), LONG(y + height / 2 - BMP_ARROW_DISPLAY_HEIGHT / 2) };
			rotPt[1] = { LONG(x + width - ARROW_MARGIN), LONG(y + height / 2 + BMP_ARROW_DISPLAY_HEIGHT / 2) };
			rotPt[2] = { LONG(x + width - ARROW_MARGIN - BMP_ARROW_DISPLAY_WIDTH), LONG(y + height / 2 - BMP_ARROW_DISPLAY_HEIGHT / 2) };
		}
		else {
			rotPt[0] = { LONG(x + width - BMP_ARROW_DISPLAY_HEIGHT - ARROW_MARGIN), LONG(y + height / 2 + BMP_ARROW_DISPLAY_HEIGHT / 2) };
			rotPt[1] = { LONG(x + width - BMP_ARROW_DISPLAY_HEIGHT - ARROW_MARGIN), LONG(y + height / 2 - BMP_ARROW_DISPLAY_HEIGHT / 2) };
			rotPt[2] = { LONG(x + width - ARROW_MARGIN), LONG(y + height / 2 + BMP_ARROW_DISPLAY_HEIGHT / 2) };
		}
		PlgBlt(hdc, rotPt, hdcBmpArrow, 0, 0, bmpArrow.bmWidth, bmpArrow.bmHeight, NULL, NULL, NULL);
	}

	//TOP side
	for (UINT i = 0; i < monitor.GetTopLeds(); i++) {
		StretchBlt(hdc,
			monitor.GetClockwiseTop() ? x + i * width / monitor.GetTopLeds() : x + width - i * width / monitor.GetTopLeds(), y,
			BMP_LED_DISPLAY_WIDTH, BMP_LED_DISPLAY_HEIGHT,
			hdcBmpLED,
			0, 0,
			bmpLED.bmWidth, bmpLED.bmHeight,
			SRCCOPY);
	}	
	if (monitor.GetTopLeds()) {
		if (monitor.GetClockwiseTop()) {
			rotPt[0] = { LONG(x + width / 2 - BMP_ARROW_DISPLAY_WIDTH / 2), LONG(y + ARROW_MARGIN)};
			rotPt[1] = { LONG(x + width / 2 + BMP_ARROW_DISPLAY_WIDTH / 2), LONG(y + ARROW_MARGIN)};
			rotPt[2] = { LONG(x + width / 2 - BMP_ARROW_DISPLAY_WIDTH / 2), LONG(y + ARROW_MARGIN + BMP_ARROW_DISPLAY_HEIGHT) };
		}
		else {
			rotPt[0] = { LONG(x + width / 2 + BMP_ARROW_DISPLAY_WIDTH / 2), LONG(y + ARROW_MARGIN + BMP_LED_DISPLAY_HEIGHT) };
			rotPt[1] = { LONG(x + width / 2 - BMP_ARROW_DISPLAY_WIDTH / 2), LONG(y + ARROW_MARGIN + BMP_LED_DISPLAY_HEIGHT) };
			rotPt[2] = { LONG(x + width / 2 + BMP_ARROW_DISPLAY_WIDTH / 2), LONG(y + ARROW_MARGIN) };
		}
		PlgBlt(hdc, rotPt, hdcBmpArrow, 0, 0, bmpArrow.bmWidth, bmpArrow.bmHeight, NULL, NULL, NULL);
	}

	//BOTTOM side
	for (UINT i = 0; i < monitor.GetBottomLeds(); i++) {
		StretchBlt(hdc,
			monitor.GetClockwiseBottom() ? x + i * width / monitor.GetBottomLeds() : x + width - i * width / monitor.GetBottomLeds(), y + height - BMP_LED_DISPLAY_HEIGHT,
			BMP_LED_DISPLAY_WIDTH, BMP_LED_DISPLAY_HEIGHT,
			hdcBmpLED,
			0, 0,
			bmpLED.bmWidth, bmpLED.bmHeight,
			SRCCOPY);
	}
	if (monitor.GetBottomLeds()) {
		if (monitor.GetClockwiseBottom()) {
			rotPt[0] = { LONG(x + width / 2 + BMP_ARROW_DISPLAY_WIDTH / 2), LONG(y + height - ARROW_MARGIN) };
			rotPt[1] = { LONG(x + width / 2 - BMP_ARROW_DISPLAY_WIDTH / 2), LONG(y + height - ARROW_MARGIN) };
			rotPt[2] = { LONG(x + width / 2 + BMP_ARROW_DISPLAY_WIDTH / 2), LONG(y + height - ARROW_MARGIN - BMP_ARROW_DISPLAY_HEIGHT) };
		}
		else {
			rotPt[0] = { LONG(x + width / 2 - BMP_ARROW_DISPLAY_WIDTH / 2), LONG(y + height - ARROW_MARGIN - BMP_ARROW_DISPLAY_HEIGHT) };
			rotPt[1] = { LONG(x + width / 2 + BMP_ARROW_DISPLAY_WIDTH / 2), LONG(y + height - ARROW_MARGIN - BMP_ARROW_DISPLAY_HEIGHT) };
			rotPt[2] = { LONG(x + width / 2 - BMP_ARROW_DISPLAY_WIDTH / 2), LONG(y + height - ARROW_MARGIN) };
		}
		PlgBlt(hdc, rotPt, hdcBmpArrow, 0, 0, bmpArrow.bmWidth, bmpArrow.bmHeight, NULL, NULL, NULL);
	}

	DeleteDC(hdcBmpArrow);
	DeleteObject(hBmpArrow);
	DeleteDC(hdcBmpLED);
	DeleteObject(hBmpLED);

	DeleteObject(hBrush);

	CONST auto MONITORNAME_MARGIN_X = 30, MONITORNAME_MARGIN_Y = 20;
	RECT monitorRect;
	monitorRect.left = x + MONITORNAME_MARGIN_X;
	monitorRect.right = x + width - MONITORNAME_MARGIN_X;
	monitorRect.top = y + MONITORNAME_MARGIN_Y;
	monitorRect.bottom = y + height - MONITORNAME_MARGIN_Y;

	DrawText(hdc, monitor.GetMonitorName().c_str(), lstrlen(monitor.GetMonitorName().c_str()), &monitorRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);
}

/*
	Loads a bitmap from the current directory.

	@param fileName: The filename of the bitmap file.
*/
CONST HBITMAP SetupGUI::LoadBMPFromCD(CONST LPCWSTR fileName) {
	WCHAR buffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, buffer);

	if (StringCchCat(buffer, MAX_PATH, fileName) != S_OK) {
		throw L"Exception in function LoadBMPFromCD. Error: Failed to concatenate file path strings.";
	}

	return (HBITMAP)LoadImage(NULL, buffer, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE); 
}