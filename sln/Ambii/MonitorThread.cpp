/*
	MonitorThread.cpp

	Source file for class MonitorThread.

	@author: Martin Terneborg
*/

#include "MonitorThread.h"

#define OUTPUT_SIZE 30

/*
	Overloaded constructor.

	@param rMonitor: A reference to the monitor this thread is associated with.
	@param hWnd: A handle to the main window.
	@param rSettings: A reference to a settings object containing the settings for the program.
	@param hdc: A HDC object to display the monitor and output in.
	@param drawWidth: The width of the area that the thread should draw in.
	@param drawHeight: The height of the area thtat the thread should draw in.
	@param outputLeft: An array where the output values of the left side of the monitor will be written.
	@param outputRight: An array where the output values of the right side of the monitor will be written.
	@param outputTop: An array where the output values of the top side of the monitor will be written.
	@param outputBottom: An array where the output values of the bottom side of the monitor will be written.

	@remarks The thread does not allocate memory or create the hdc itself, this needs to be done before constructing and running the thread.
*/
MonitorThread::MonitorThread(CONST Monitor &rMonitor, CONST HWND hWnd, CONST Settings &rSettings, CONST HDC hdc, CONST UINT drawWidth, CONST UINT drawHeight,
	RGBQUAD *CONST outputLeft, RGBQUAD *CONST outputRight, RGBQUAD *CONST outputTop, RGBQUAD *CONST outputBottom)
	: m_rMonitor(rMonitor), m_hWnd(hWnd), m_rSettings(rSettings), m_hdc(hdc), m_drawWidth(drawWidth), m_drawHeight(drawHeight),
	m_outputLeft(outputLeft), m_outputRight(outputRight), m_outputTop(outputTop), m_outputBottom(outputBottom)
{
	m_arrPixels = std::make_unique<RGBQUAD[]>(rMonitor.GetWidth() * rMonitor.GetHeight());
}

/*
	Runs (starts) thread.
*/
VOID MonitorThread::Run() {
	m_rMonitor.GetPixels(m_arrPixels.get());

	if(m_rSettings.m_bDisplayMonitors){
		DisplayMonitor();
	}

	if(m_rMonitor.GetEnableLeft())
		CalculateLedsLeft();
	if (m_rMonitor.GetEnableRight())
		CalculateLedsRight();
	if (m_rMonitor.GetEnableTop())
		CalculateLedsTop();
	if (m_rMonitor.GetEnableBottom())
		CalculateLedsBottom();

	if (m_rSettings.m_bDisplayOutput) {
		DisplayLedsLeft();
		DisplayLedsRight();
		DisplayLedsTop();
		DisplayLedsBottom();
	}
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
		m_rMonitor.GetPosX() * m_drawWidth, m_drawHeight * m_rMonitor.GetPosY(), //Horizontal and vertical position of destionation
		m_drawWidth, m_drawHeight, //Width and height of destination
		0, 0, //Horizontal and vertical position of source
		m_rMonitor.GetWidth(), m_rMonitor.GetHeight(), //Width and height of source
		m_arrPixels.get(),
		&bmi, DIB_RGB_COLORS, SRCCOPY); //Draw to the window

	DeleteDC(hdcMonitor);
}

/*
	Calculates the LED output values on the left side of the monitor.
*/
VOID MonitorThread::CalculateLedsLeft() {
	RECT clientRect;
	GetClientRect(m_hWnd, &clientRect);

	UINT monitorWidth = m_rMonitor.GetWidth();
	UINT monitorHeight = m_rMonitor.GetHeight();

	for (UINT i = 0; i < m_rMonitor.GetLeftLeds(); i++) {
		UINT rSum = 0, gSum = 0, bSum = 0;
		for (UINT x = 0; x < m_rSettings.m_sampleSize; x++) {
			for (UINT y = 0; y < m_rSettings.m_sampleSize; y++) {
				RGBQUAD p;
				//If clockwise, the values need to be calculated in a bottom-top order
				if (m_rMonitor.GetClockwiseLeft()) {
					p = GetPixelFromArr(
						x, monitorHeight - i * monitorHeight / m_rMonitor.GetLeftLeds() - y,
						m_arrPixels.get(), monitorWidth);
				}
				else { //Otherwise the order is top-bottom
					p = GetPixelFromArr(x, i * monitorHeight / m_rMonitor.GetLeftLeds() + y,
						m_arrPixels.get(), monitorWidth);
				}
				rSum += p.rgbRed;
				gSum += p.rgbGreen;
				bSum += p.rgbBlue;
			}
		}

		//Calculate the average of the color channels
		UINT nSampleLeds = m_rSettings.m_sampleSize * m_rSettings.m_sampleSize;
		BYTE rAvg = rSum / nSampleLeds, gAvg = gSum / nSampleLeds, bAvg = bSum / nSampleLeds;
		m_outputLeft[i] = RGBQUAD{ bAvg, gAvg, rAvg, 0 }; //RGBQUAD quad is defined as {b,g,r, reserved}
	}
}

/*
	Calculates the LED output values on the right side of the monitor.
*/
VOID MonitorThread::CalculateLedsRight() {
	RECT clientRect;
	GetClientRect(m_hWnd, &clientRect);

	UINT monitorWidth = m_rMonitor.GetWidth();
	UINT monitorHeight = m_rMonitor.GetHeight();

	for (UINT i = 0; i < m_rMonitor.GetRightLeds(); i++) {
		UINT rSum = 0, gSum = 0, bSum = 0;
		for (UINT x = m_rMonitor.GetWidth() - 1; x > m_rMonitor.GetWidth() - 1 - m_rSettings.m_sampleSize; x--) {
			for (UINT y = 0; y < m_rSettings.m_sampleSize; y++) {
				RGBQUAD p;
				//If clockwise, the values need to be calculated in a top-bottom order
				if (m_rMonitor.GetClockwiseRight()) {
					p = GetPixelFromArr(
						x, i * monitorHeight / m_rMonitor.GetRightLeds() + y,
						m_arrPixels.get(), monitorWidth);
				}
				else { //Otherwise the order is bottom-top
					p = GetPixelFromArr(
						x, monitorHeight - (i + 1) * monitorHeight / m_rMonitor.GetRightLeds() + y,
						m_arrPixels.get(), monitorWidth);
				}
				rSum += p.rgbRed;
				gSum += p.rgbGreen;
				bSum += p.rgbBlue;
			}
		}

		//Calculate the average of the color channels
		UINT nSampleLeds = m_rSettings.m_sampleSize * m_rSettings.m_sampleSize;
		BYTE rAvg = rSum / nSampleLeds, gAvg = gSum / nSampleLeds, bAvg = bSum / nSampleLeds;
		m_outputRight[i] = RGBQUAD{ bAvg, gAvg, rAvg, 0 }; //RGBQUAD is defined as {b,g,r, reserved}
	}
}

/*
	Calculates the LED output values on the top side of the monitor.
*/
VOID MonitorThread::CalculateLedsTop() {
	RECT clientRect;
	GetClientRect(m_hWnd, &clientRect);

	UINT monitorWidth = m_rMonitor.GetWidth();
	UINT monitorHeight = m_rMonitor.GetHeight();

	for (UINT i = 0; i < m_rMonitor.GetTopLeds(); i++) {
		UINT rSum = 0, gSum = 0, bSum = 0;
		for (UINT x = 0; x < m_rSettings.m_sampleSize; x++) {
			for (UINT y = 0; y < m_rSettings.m_sampleSize; y++) {
				RGBQUAD p;
				//If clockwise, the output need to be calculated in a left-right order
				if (m_rMonitor.GetClockwiseTop()) {
					p = GetPixelFromArr(
						i * monitorWidth / m_rMonitor.GetTopLeds(), y,
						m_arrPixels.get(), monitorWidth);
				}
				else { //Otherwise the order is right-left
					p = GetPixelFromArr(
						monitorWidth - (i + 1) * monitorWidth / m_rMonitor.GetTopLeds(), y,
						m_arrPixels.get(), monitorWidth);
				}
				rSum += p.rgbRed;
				gSum += p.rgbGreen;
				bSum += p.rgbBlue;
			}
		}

		//Calculate the average of the color channels
		UINT nSampleLeds = m_rSettings.m_sampleSize * m_rSettings.m_sampleSize;
		BYTE rAvg = rSum / nSampleLeds, gAvg = gSum / nSampleLeds, bAvg = bSum / nSampleLeds;
		m_outputTop[i] = RGBQUAD{ bAvg, gAvg, rAvg, 0 }; //RGBQUAD is defined as {b,g,r, reserved}
	}
}

/*
	Calculates the LED output values on the bottom side of the monitor.
*/
VOID MonitorThread::CalculateLedsBottom() {
	RECT clientRect;
	GetClientRect(m_hWnd, &clientRect);

	UINT monitorWidth = m_rMonitor.GetWidth();
	UINT monitorHeight = m_rMonitor.GetHeight();

	for (UINT i = 0; i < m_rMonitor.GetBottomLeds(); i++) {
		UINT rSum = 0, gSum = 0, bSum = 0;
		for (UINT x = 0; x < m_rSettings.m_sampleSize; x++) {
			for (UINT y = m_rMonitor.GetHeight() - 1; y > m_rMonitor.GetHeight() - 1 - m_rSettings.m_sampleSize; y--) {
				RGBQUAD p;
				//If clockwise, the values need to be calculated in a right-left order
				if (m_rMonitor.GetClockwiseBottom()) {
					p = GetPixelFromArr(
						monitorWidth - (i + 1) * monitorWidth / m_rMonitor.GetBottomLeds(), y,
						m_arrPixels.get(), monitorWidth);
				}
				else { //Otherwise the order is left-right
					p = GetPixelFromArr(
						i * monitorWidth / m_rMonitor.GetBottomLeds() + x, y,
						m_arrPixels.get(), monitorWidth);
				}
				rSum += p.rgbRed;
				gSum += p.rgbGreen;
				bSum += p.rgbBlue;
			}
		}

		//Calculate the average of the color channels
		UINT nSampleLeds = m_rSettings.m_sampleSize * m_rSettings.m_sampleSize;
		BYTE rAvg = rSum / nSampleLeds, gAvg = gSum / nSampleLeds, bAvg = bSum / nSampleLeds;
		m_outputBottom[i] = RGBQUAD{ bAvg, gAvg, rAvg, 0 }; //RGBQUAD is defined as {b,g,r, reserved}
	}
}

/*
	Displays the LED output values on the left side of the monitor.
*/
VOID MonitorThread::DisplayLedsLeft() {
	RECT clientRect;
	GetClientRect(m_hWnd, &clientRect);

	UINT x = m_rMonitor.GetPosX() * clientRect.right / m_rSettings.m_usedMonitors.size();
	for (UINT i = 0; i < m_rMonitor.GetLeftLeds(); i++) {
		UINT y = m_drawHeight * m_rMonitor.GetPosY() + i * (m_drawHeight - OUTPUT_SIZE) / (m_rMonitor.GetLeftLeds() - 1);

		CONST UINT index = m_rMonitor.GetClockwiseLeft() ? m_rMonitor.GetLeftLeds() - 1 - i : i;
		HBRUSH hBrush = CreateSolidBrush(RGB(m_outputLeft[index].rgbRed, m_outputLeft[index].rgbGreen, m_outputLeft[index].rgbBlue));
		SelectObject(m_hdc, hBrush);

		Rectangle(m_hdc, x, y, x + OUTPUT_SIZE, y + OUTPUT_SIZE);
		DeleteObject(hBrush);
	}
}

/*
	Displays the LED output values on the right side of the monitor.
*/
VOID MonitorThread::DisplayLedsRight() {
	RECT clientRect;
	GetClientRect(m_hWnd, &clientRect);

	UINT x = (m_rMonitor.GetPosX() + 1) * clientRect.right / m_rSettings.m_usedMonitors.size() - OUTPUT_SIZE;
	for (UINT i = 0; i < m_rMonitor.GetRightLeds(); i++) {
		UINT y = m_drawHeight * m_rMonitor.GetPosY() + i * (m_drawHeight - OUTPUT_SIZE) / (m_rMonitor.GetRightLeds() - 1);

		CONST UINT index = m_rMonitor.GetClockwiseRight() ? i : m_rMonitor.GetRightLeds() - 1 - i;
		HBRUSH hBrush = CreateSolidBrush(RGB(m_outputRight[index].rgbRed, m_outputRight[index].rgbGreen, m_outputRight[index].rgbBlue));
		SelectObject(m_hdc, hBrush);

		Rectangle(m_hdc, x, y, x + OUTPUT_SIZE, y + OUTPUT_SIZE);
		DeleteObject(hBrush);
	}
}

/*
	Displays the LED output values on the top side of the monitor.
*/
VOID MonitorThread::DisplayLedsTop() {
	RECT clientRect;
	GetClientRect(m_hWnd, &clientRect);

	UINT y = m_drawHeight * m_rMonitor.GetPosY();
	for (UINT i = 0; i < m_rMonitor.GetTopLeds(); i++) {
		UINT x = m_drawWidth * m_rMonitor.GetPosX() + i * (m_drawWidth - OUTPUT_SIZE) / (m_rMonitor.GetTopLeds() - 1);

		CONST UINT index = m_rMonitor.GetClockwiseTop() ? i : m_rMonitor.GetTopLeds() - 1 - i;
		HBRUSH hBrush = CreateSolidBrush(RGB(m_outputTop[index].rgbRed, m_outputTop[index].rgbGreen, m_outputTop[index].rgbBlue));
		SelectObject(m_hdc, hBrush);

		Rectangle(m_hdc, x, y, x + OUTPUT_SIZE, y + OUTPUT_SIZE);
		DeleteObject(hBrush);
	}
}

/*		
	Displays the LED output values on the bottom side of the monitor.
*/
VOID MonitorThread::DisplayLedsBottom() {
	RECT clientRect;
	GetClientRect(m_hWnd, &clientRect);

	UINT y = m_drawHeight * (m_rMonitor.GetPosY() + 1) - OUTPUT_SIZE;
	for (UINT i = 0; i < m_rMonitor.GetBottomLeds(); i++) {
		UINT x = m_drawWidth * m_rMonitor.GetPosX() + i * (m_drawWidth - OUTPUT_SIZE) / (m_rMonitor.GetBottomLeds() - 1);

		CONST UINT index = m_rMonitor.GetClockwiseBottom() ? m_rMonitor.GetBottomLeds() - 1 - i : i;
		HBRUSH hBrush = CreateSolidBrush(RGB(m_outputBottom[index].rgbRed, m_outputBottom[index].rgbGreen, m_outputBottom[index].rgbBlue));
		SelectObject(m_hdc, hBrush);

		Rectangle(m_hdc, x, y, x + OUTPUT_SIZE - 1, y + OUTPUT_SIZE);
		DeleteObject(hBrush);
	}
}