/*
	OutputThread.h

	Header file for class OutputThread.

	@author: Martin Terneborg.
*/

#ifndef OUTPUT_THREAD_H
#define OUTPUT_THREAD_H

#include "Thread.h"

#include <Windows.h>

#include "Monitor.h"
#include "Settings.h"

/*
	Class for representing a thread responsible for calculating and displaying the output.
*/
class OutputThread : public Thread {
private:
	CONST HWND m_hWnd;
	CONST Monitor &m_rMonitor;
	CONST RGBQUAD * CONST m_arrPixels;
	CONST Settings &m_rSettings;

	VOID Run();
	VOID CalculateLeftLeds(RGBQUAD * CONST arrPixels) CONST;
	VOID CalculateRightLeds(RGBQUAD * CONST arrPixels) CONST;
	VOID CalculateTopLeds(RGBQUAD * CONST arrPixels) CONST;
	VOID CalculateBottomLeds(RGBQUAD * CONST arrPixels) CONST;
	VOID CalculateLeds(RGBQUAD * CONST arrOutput, CONST UINT nSide) CONST;
	VOID DisplayLeftLeds(CONST RGBQUAD * CONST arrPixels) CONST;
	VOID DisplayRightLeds(CONST RGBQUAD * CONST arrPixels) CONST;
	VOID DisplayTopLeds(CONST RGBQUAD * CONST arrPixels) CONST;
	VOID DisplayBottomLeds(CONST RGBQUAD * CONST arrPixels) CONST;

public:
	OutputThread(CONST HWND hWnd, CONST Monitor &pMonitor, CONST RGBQUAD * arrPixels, CONST Settings &m_pSettings);
};

#endif