/*
	//TODO: Comment
*/

#include "InitCtrls.h"

#include <CommCtrl.h>

/*
	Creates a static control with text.

	@param hWndParent: A handle to a parent window
	@param text: The text of the control.
	@param x: The horizontal position of the control.
	@param y: The vertical position of the control.

	@return A handle to the newly created control or NULL if the creation failed.
*/
CONST HWND InitTextCtrl(CONST HWND hWndParent, CONST LPCWSTR text, CONST UINT x, CONST UINT y) {

	//Create a DC in memory to hold the font
	HDC hdcMem = CreateCompatibleDC(NULL);
	SelectObject(hdcMem, (HBRUSH)GetStockObject(DEFAULT_GUI_FONT));

	//Find out the required size of the text control
	SIZE textSize;
	GetTextExtentPoint32(hdcMem, text, lstrlen(text), &textSize);
	DeleteDC(hdcMem);

	HWND hText = CreateWindow(
		WC_STATIC,
		text,
		WS_CHILD | WS_VISIBLE,
		x, y,
		textSize.cx, textSize.cy,
		hWndParent,
		NULL,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hText, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);

	return hText;
}

/*
	Creates an edit control.

	@param hWndParent: A handle to a parent window
	@param x: The horizontal position of the control.
	@param y: The vertical position of the control.
	@param width: The width of the control.
	@param height: The height of the control.
	@param id: The id of the control.

	@return A handle to the newly created control or NULL if the creation failed.
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
	Creates a checkbox(button) control.

	@param hWndParent: A handle to a parent window
	@param x: The horizontal position of the control.
	@param y: The vertical position of the control.
	@param text: The text to be displayed next to the checkbox.
	@param id: The id of the control.

	@return A handle to the newly created control or NULL if the creation failed.
*/
CONST HWND InitCheckboxCtrl(CONST HWND hWndParent, CONST UINT x, CONST UINT y, CONST LPCWSTR text, CONST HMENU id) {
	//Create a DC in memory to hold the font
	HDC hdcMem = CreateCompatibleDC(NULL);
	SelectObject(hdcMem, (HBRUSH)GetStockObject(DEFAULT_GUI_FONT));
	
	//Find out the required size of the text control
	SIZE textSize;
	GetTextExtentPoint32(hdcMem, text, lstrlen(text), &textSize);
	DeleteDC(hdcMem);

	HWND hCheckbox = CreateWindow(
		WC_BUTTON, text,
		WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_LEFTTEXT,
		x, y,
		textSize.cx + 2 * GetSystemMetrics(SM_CXMENUCHECK), textSize.cy,
		hWndParent,
		id,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hCheckbox, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);

	return hCheckbox;
}