/*
	//TODO: Comment
*/

#include "InitCtrls.h"

/*
	//TODO: Comment
*/
CONST HWND InitTextCtrl(CONST HWND hWndParent, CONST LPCWSTR text, CONST UINT x, CONST UINT y) {
	//Create a DC in memory to hold the font
	HDC hdcMem = CreateCompatibleDC(NULL);
	SelectObject(hdcMem, (HBRUSH)GetStockObject(DEFAULT_GUI_FONT));
	SIZE size;
	GetTextExtentPoint32(hdcMem, EDITTEXT_POSITION_BOTTOM, lstrlen(EDITTEXT_POSITION_BOTTOM), &size);
	DeleteDC(hdcMem);

	HWND hText = CreateWindow(
		WC_STATIC,
		text,
		WS_CHILD | WS_VISIBLE,
		x, y,
		size.cx, size.cy,
		hWndParent,
		NULL,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hText, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);

	return hText;
}

/*
	//TODO: Comment
*/
CONST HWND InitEditCtrl(CONST HWND hWndParent, CONST UINT x, CONST UINT y, CONST UINT width, CONST UINT height, CONST HMENU id) {
	HWND hEdit = CreateWindow(
		WC_EDIT, L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
		x, y,
		width, height,
		hWndParent,
		id,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hEdit, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);

	return hEdit;
}

/*
	TODO: Comment
*/
CONST HWND InitCheckboxCtrl(CONST HWND hWndParent, CONST UINT x, CONST UINT y, CONST LPCWSTR text, CONST HMENU id) {
	//Create a DC in memory to hold the font
	HDC hdcMem = CreateCompatibleDC(NULL);
	SelectObject(hdcMem, (HBRUSH)GetStockObject(DEFAULT_GUI_FONT));
	SIZE size;
	GetTextExtentPoint32(hdcMem, EDITTEXT_POSITION_BOTTOM, lstrlen(EDITTEXT_POSITION_BOTTOM), &size);
	DeleteDC(hdcMem);

	HWND hCheckbox = CreateWindow(
		WC_BUTTON, text,
		WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_LEFTTEXT,
		x, y,
		size.cx + 2 * GetSystemMetrics(SM_CXMENUCHECK), size.cy,
		hWndParent,
		id,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hCheckbox, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);

	return hCheckbox;
}