/*
	MonitorThread.cpp

	Source file for class MonitorThread.

	@author: Martin Terneborg
*/

#include "MonitorThread.h"

#include "OutputThread.h"

#define OUTPUT_SIZE 15

/*
	//TODO: Comment
*/
MonitorThread::MonitorThread(CONST Monitor &rMonitor, CONST HWND hWnd, CONST Settings &rSettings, CONST HDC hdc, RGBQUAD * CONST output)
	: m_rMonitor(rMonitor), m_hWnd(hWnd), m_rSettings(rSettings), m_hdc(hdc), m_output(output)
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
				if (m_rSettings.m_bClockwise) {
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
		m_output[i] = RGBQUAD{ bAvg, gAvg, rAvg, 0 }; //RGBQUAD quad is defined as {b,g,r, reserved}
	}
}

/*
	//TODO: Comment
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
				if (m_rSettings.m_bClockwise) {
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
		m_output[i] = RGBQUAD{ bAvg, gAvg, rAvg, 0 }; //RGBQUAD quad is defined as {b,g,r, reserved}
	}
}

/*
	//TODO: Comment
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
				if (m_rSettings.m_bClockwise) {
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
		m_output[i] = RGBQUAD{ bAvg, gAvg, rAvg, 0 }; //RGBQUAD quad is defined as {b,g,r, reserved}
	}
}

/*
	//TODO: Comment
*/
VOID MonitorThread::CalculateLedsBottom() {
	if (m_rMonitor.GetBottomLeds() == 0)
		return;

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
				if (m_rSettings.m_bClockwise) {
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
		m_output[i] = RGBQUAD{ bAvg, gAvg, rAvg, 0 }; ////RGBQUAD quad is defined as {b,g,r, reserved}
	}
}

/*
	//TODO: Comment
*/
VOID MonitorThread::DisplayLedsLeft() {
	if (m_rMonitor.GetLeftLeds() == 0)
		return;

	HDC hdcWnd = GetDC(m_hWnd);

	RECT clientRect;
	GetClientRect(m_hWnd, &clientRect);

	for (UINT i = 0; i < m_rMonitor.GetLeftLeds(); i++) {
		HBRUSH hBrush = CreateSolidBrush(RGB(arrOutputs[i].rgbRed, arrOutputs[i].rgbGreen, arrOutputs[i].rgbBlue));
		SelectObject(hdcWnd, hBrush);
		UINT x = m_rMonitor.GetPosX() * clientRect.right / m_rSettings.m_usedMonitors.size();

		UINT y;
		if (m_rSettings.m_bClockwise == TRUE) {
			y = clientRect.bottom - (i + 1) * clientRect.bottom / m_rMonitor.GetLeftLeds();
		}
		else {
			y = i * clientRect.bottom / m_rMonitor.GetLeftLeds();
		}

		Rectangle(hdcWnd, x, y, x + OUTPUT_SIZE, y + OUTPUT_SIZE);
		DeleteObject(hBrush);
	}

	ReleaseDC(m_hWnd, hdcWnd);
}

/*
	//TODO: Comment
*/
VOID MonitorThread::DisplayLedsRight() {
	if (m_rMonitor.GetRightLeds() == 0)
		return;

	HDC hdcWnd = GetDC(m_hWnd);

	RECT clientRect;
	GetClientRect(m_hWnd, &clientRect);

	for (UINT i = 0; i < m_rMonitor.GetRightLeds(); i++) {
		HBRUSH hBrush = CreateSolidBrush(RGB(arrOutputs[i].rgbRed, arrOutputs[i].rgbGreen, arrOutputs[i].rgbBlue));
		SelectObject(hdcWnd, hBrush);
		UINT x = (m_rMonitor.GetPosX() + 1) * clientRect.right / m_rSettings.m_usedMonitors.size();

		UINT y;
		if (m_rSettings.m_bClockwise == TRUE) {
			y = i * clientRect.bottom / m_rMonitor.GetRightLeds();
		}
		else {
			y = clientRect.bottom - (i + 1) * clientRect.bottom / m_rMonitor.GetRightLeds();
		}

		Rectangle(hdcWnd, x - OUTPUT_SIZE, y, x, y + OUTPUT_SIZE);
		DeleteObject(hBrush);
	}

	ReleaseDC(m_hWnd, hdcWnd);
}

/*
	//TODO: Comment
*/
VOID MonitorThread::DisplayLedsTop() {
	if (m_rMonitor.GetTopLeds() == 0)
		return;

	HDC hdcWnd = GetDC(m_hWnd);

	RECT clientRect;
	GetClientRect(m_hWnd, &clientRect);

	for (UINT i = 0; i < m_rMonitor.GetTopLeds(); i++) {
		HBRUSH hBrush = CreateSolidBrush(RGB(arrOutputs[i].rgbRed, arrOutputs[i].rgbGreen, arrOutputs[i].rgbBlue));
		SelectObject(hdcWnd, hBrush);

		UINT x;
		if (m_rSettings.m_bClockwise == TRUE) {
			x = i * clientRect.right / m_rMonitor.GetTopLeds();
		}
		else {
			x = clientRect.right - (i + 1) * clientRect.right / m_rMonitor.GetTopLeds();
		}


		Rectangle(hdcWnd, x, 0, x + OUTPUT_SIZE, OUTPUT_SIZE);
		DeleteObject(hBrush);
	}

	ReleaseDC(m_hWnd, hdcWnd);
}

/*
	//TODO: Comment
*/
VOID MonitorThread::DisplayLedsBottom() {
	if (m_rMonitor.GetBottomLeds() == 0)
		return;

	HDC hdcWnd = GetDC(m_hWnd);

	RECT clientRect;
	GetClientRect(m_hWnd, &clientRect);

	for (UINT i = 0; i < m_rMonitor.GetBottomLeds(); i++) {
		HBRUSH hBrush = CreateSolidBrush(RGB(arrOutputs[i].rgbRed, arrOutputs[i].rgbGreen, arrOutputs[i].rgbBlue));
		SelectObject(hdcWnd, hBrush);

		UINT x;
		if (m_rSettings.m_bClockwise == TRUE) {
			x = clientRect.right - (i + 1) * clientRect.right / m_rMonitor.GetBottomLeds();
		}
		else {
			x = i * clientRect.right / m_rMonitor.GetBottomLeds();
		}

		Rectangle(hdcWnd, x, clientRect.bottom - OUTPUT_SIZE, x + OUTPUT_SIZE, clientRect.bottom);
		DeleteObject(hBrush);
	}

	ReleaseDC(m_hWnd, hdcWnd);
}