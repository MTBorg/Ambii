/*
	WndMain.h

	Header file for class MainWnd.

	@author: Martin Terneborg.
*/

#ifndef MAINWND_H
#define MAINWND_H

#include <Windows.h>

#include "Settings.h"
#include "SetupWnd.h"
#include "UpdateThread.h"
#include "SerialHandler.h"

/*
	Class for the main window of the program.
*/
class WndMain {
private:
	enum m_MENU_ITEM { //Identifiers of menu items
		FILE_EXIT = 1,
		VIEW_MONITORS,
		VIEW_GUIDE,
		VIEW_OUTPUT,
		EDIT_SETTINGS,
		EDIT_SETUP,
		SERIAL_SELECT,
		SERIAL_RECONNECT,
	};

	static CONST LPCWSTR m_APP_TITLE;
	Settings m_settings;
	UpdateThread * m_pUpdateThread;
	HANDLE m_hUpdateThread;
	SetupWnd m_setupWnd;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	VOID InitMenu(CONST HWND hWnd);
	VOID UpdateSingleThread(CONST HWND hWnd);
public:
	WndMain();
	~WndMain();

	VOID Register();
	HWND Create();
	VOID Run(CONST HWND hWnd);
};

#endif