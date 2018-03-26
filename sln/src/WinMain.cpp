/*
	WinMain.cpp

	Entry-point of the application.

	@author: Martin Terneborg.
*/

#include <Windows.h>

#include "WndMain.h"

//Use unicode UTF-16
#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef UNICODE
#define UNICODE
#endif

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
	INT nCmdShow) {
	HWND hWndMain;
	WndMain wndMain;
	try {

		wndMain.Register();
		hWndMain = wndMain.Create();
		wndMain.Run(hWndMain);
	}
	catch (LPCWSTR errorMsg) {
		MessageBox(NULL, errorMsg, L"Error", MB_ICONERROR);
		return 0;
	}
	return 0;
}