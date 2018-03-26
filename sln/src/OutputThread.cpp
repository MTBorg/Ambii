/*
	OutputThread.cpp

	Source file for class OutputThread.

	@author: Martin Terneborg.
*/

#include "OutputThread.h"

#include "SerialHandler.h"

#define OUTPUT_SIZE 15

inline RGBQUAD GetPixelFromArr(CONST UINT x, CONST UINT y, CONST RGBQUAD *CONST arrPixels, CONST UINT width);

/*
	Overloaded constructor.

	@param hWnd: A handle to the window to draw to.
	@param rMonitor: A reference to the monitor to use.
	@param arrPixels: An array containing the monitor pixels.
	@param rSettings: A reference to the settings object of the program,
*/
OutputThread::OutputThread(CONST HWND hWnd, CONST Monitor &rMonitor, CONST RGBQUAD * arrPixels, CONST Settings &rSettings)
	: m_hWnd(hWnd), m_rMonitor(rMonitor), m_arrPixels(arrPixels), m_rSettings(rSettings)
{

}

/*
	Runs the thread which calculates and display the output.
*/
VOID OutputThread::Run() {
	CONST LONG totalSize = m_rMonitor.GetLeftLeds() + m_rMonitor.GetRightLeds()
		+ m_rMonitor.GetTopLeds() + m_rMonitor.GetBottomLeds();
	RGBQUAD * output = new RGBQUAD[totalSize];

	if (m_rSettings.m_bClockwise) {
		CalculateLeftLeds(output);
		CalculateTopLeds(output + m_rMonitor.GetLeftLeds());
		CalculateRightLeds(output + m_rMonitor.GetLeftLeds() + m_rMonitor.GetTopLeds());
		CalculateBottomLeds(output + m_rMonitor.GetLeftLeds() +
			m_rMonitor.GetTopLeds() + m_rMonitor.GetRightLeds());
	}
	else {
		CalculateLeftLeds(output);
		CalculateBottomLeds(output + m_rMonitor.GetLeftLeds());
		CalculateRightLeds(output + m_rMonitor.GetLeftLeds()
			+ m_rMonitor.GetBottomLeds());
		CalculateTopLeds(output + m_rMonitor.GetLeftLeds()
			+ m_rMonitor.GetBottomLeds() + m_rMonitor.GetRightLeds());
	}

	if (m_rSettings.m_bDisplayOutput) {
		DisplayLeftLeds(output);
		DisplayTopLeds(output + m_rMonitor.GetLeftLeds());
		DisplayRightLeds(output + m_rMonitor.GetLeftLeds() + m_rMonitor.GetTopLeds());
		DisplayBottomLeds(output + m_rMonitor.GetLeftLeds() +
			m_rMonitor.GetTopLeds() + m_rMonitor.GetRightLeds());
	}

	if (!SerialHandler::Stopped()) {
		SerialHandler::WriteToPort((BYTE*)output, totalSize * 4, m_rSettings.m_portNum);
	}

	delete[] output;
}

/*
	Calculates the output values for the left side of the monitor.

	@param arrOutput: An array where the output values will be stored.
*/
VOID OutputThread::CalculateLeftLeds(RGBQUAD * CONST arrOutput) CONST {
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
						m_arrPixels, monitorWidth);
				}
				else { //Otherwise the order is top-bottom
					p = GetPixelFromArr(x, i * monitorHeight / m_rMonitor.GetLeftLeds() + y,
						m_arrPixels, monitorWidth);
				}
				rSum += p.rgbRed;
				gSum += p.rgbGreen;
				bSum += p.rgbBlue;
			}
		}

		//Calculate the average of the color channels
		UINT nSampleLeds = m_rSettings.m_sampleSize * m_rSettings.m_sampleSize;
		BYTE rAvg = rSum / nSampleLeds, gAvg = gSum / nSampleLeds, bAvg = bSum / nSampleLeds;
		arrOutput[i] = RGBQUAD{ bAvg, gAvg, rAvg, 0 }; //RGBQUAD quad is defined as {b,g,r, reserved}
	}
}

/*
	Calculates the output values for the right side of the monitor.

	@param arrOutput: An array where the output values will be stored.
*/
VOID OutputThread::CalculateRightLeds(RGBQUAD * CONST arrOutput) CONST {
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
						m_arrPixels, monitorWidth);
				}
				else { //Otherwise the order is bottom-top
					p = GetPixelFromArr(
						x, monitorHeight - (i + 1) * monitorHeight / m_rMonitor.GetRightLeds() + y,
						m_arrPixels, monitorWidth);
				}
				rSum += p.rgbRed;
				gSum += p.rgbGreen;
				bSum += p.rgbBlue;
			}
		}

		//Calculate the average of the color channels
		UINT nSampleLeds = m_rSettings.m_sampleSize * m_rSettings.m_sampleSize;
		BYTE rAvg = rSum / nSampleLeds, gAvg = gSum / nSampleLeds, bAvg = bSum / nSampleLeds;
		arrOutput[i] = RGBQUAD{ bAvg, gAvg, rAvg, 0 }; //RGBQUAD quad is defined as {b,g,r, reserved}
	}
}

/*
	Calculates the output values for the top side of the monitor.

	@param arrOutput: An array where the output values will be stored.
*/
VOID OutputThread::CalculateTopLeds(RGBQUAD * CONST arrOutput) CONST {
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
						m_arrPixels, monitorWidth);
				}
				else { //Otherwise the order is right-left
					p = GetPixelFromArr(
						monitorWidth - (i + 1) * monitorWidth / m_rMonitor.GetTopLeds(), y,
						m_arrPixels, monitorWidth);
				}
				rSum += p.rgbRed;
				gSum += p.rgbGreen;
				bSum += p.rgbBlue;
			}
		}

		//Calculate the average of the color channels
		UINT nSampleLeds = m_rSettings.m_sampleSize * m_rSettings.m_sampleSize;
		BYTE rAvg = rSum / nSampleLeds, gAvg = gSum / nSampleLeds, bAvg = bSum / nSampleLeds;
		arrOutput[i] = RGBQUAD{ bAvg, gAvg, rAvg, 0 }; //RGBQUAD quad is defined as {b,g,r, reserved}
	}
}

/*
	Calculates the output values for the bottom side of the monitor.

	@param arrOutput: An array where the output values will be stored.
*/
VOID OutputThread::CalculateBottomLeds(RGBQUAD * CONST arrOutput) CONST {
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
						m_arrPixels, monitorWidth);
				}
				else { //Otherwise the order is left-right
					p = GetPixelFromArr(
						i * monitorWidth / m_rMonitor.GetBottomLeds() + x, y,
						m_arrPixels, monitorWidth);
				}
				rSum += p.rgbRed;
				gSum += p.rgbGreen;
				bSum += p.rgbBlue;
			}
		}

		//Calculate the average of the color channels
		UINT nSampleLeds = m_rSettings.m_sampleSize * m_rSettings.m_sampleSize;
		BYTE rAvg = rSum / nSampleLeds, gAvg = gSum / nSampleLeds, bAvg = bSum / nSampleLeds;
		arrOutput[i] = RGBQUAD{ bAvg, gAvg, rAvg, 0 }; ////RGBQUAD quad is defined as {b,g,r, reserved}
	}
}

/*
	Displays the output values on the left side of the monitor.

	@param arrOutputs: An array containing the output values.
*/
VOID OutputThread::DisplayLeftLeds(CONST RGBQUAD * CONST arrOutputs) CONST {
	if (m_rMonitor.GetLeftLeds() == 0)
		return;

	HDC hdcWnd = GetDC(m_hWnd);

	RECT clientRect;
	GetClientRect(m_hWnd, &clientRect);

	for (UINT i = 0; i < m_rMonitor.GetLeftLeds(); i++) {
		HBRUSH hBrush = CreateSolidBrush(RGB(arrOutputs[i].rgbRed, arrOutputs[i].rgbGreen, arrOutputs[i].rgbBlue));
		SelectObject(hdcWnd, hBrush);
		UINT x = m_rMonitor.GetPosition() * clientRect.right / m_rSettings.m_usedMonitors.size();

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
	Displays the output values on the right side of the monitor.

	@param arrOutputs: An array containing the output values.
*/
VOID OutputThread::DisplayRightLeds(CONST RGBQUAD * CONST arrOutputs) CONST {
	if (m_rMonitor.GetRightLeds() == 0)
		return;

	HDC hdcWnd = GetDC(m_hWnd);

	RECT clientRect;
	GetClientRect(m_hWnd, &clientRect);

	for (UINT i = 0; i < m_rMonitor.GetRightLeds(); i++) {
		HBRUSH hBrush = CreateSolidBrush(RGB(arrOutputs[i].rgbRed, arrOutputs[i].rgbGreen, arrOutputs[i].rgbBlue));
		SelectObject(hdcWnd, hBrush);
		UINT x = (m_rMonitor.GetPosition() + 1) * clientRect.right / m_rSettings.m_usedMonitors.size();

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
	Displays the output values on the top side of the monitor.

	@param arrOutputs: An array containing the output values.
*/
VOID OutputThread::DisplayTopLeds(CONST RGBQUAD * CONST arrOutputs) CONST {
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
	Displays the output values on the bottom side of the monitor.

	@param arrOutputs: An array containing the output values.
*/
VOID OutputThread::DisplayBottomLeds(CONST RGBQUAD * CONST arrOutputs) CONST {
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

/*
	Retrieves the pixel at the specified position from a one-dimensional array representing a 2D monitor.

	@param x: The x-cordinate of the pixel.
	@param y: The y-cordinate of the pixel.
	@param arrPixels: The pixel array.
	@param width: The width of the monitor.

	@return The RGBQUAD object for the pixel at the specified position.
*/
inline RGBQUAD GetPixelFromArr(CONST UINT x, CONST UINT y, CONST RGBQUAD *CONST arrPixels, CONST UINT width) {
	return arrPixels[y * width + x];
}