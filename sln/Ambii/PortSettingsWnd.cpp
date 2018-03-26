/*
	PortSettingsWnd.cpp

	Source file for class PortSettingsWnd.

	@author: Martin Terneborg.
*/

#include "PortSettingsWnd.h"

#include <CommCtrl.h>

CONST LPCWSTR PortSettingsWnd::m_TITLE = L"Select port";

/*
	The window procedure for the window.

	@param hWnd: A handle to the window.
	@param msg: The message to process.
	@param wParam: Word message parameter.
	@param lParam: Long message parameter.

	@return The result of the message processing.
*/
LRESULT CALLBACK PortSettingsWnd::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CREATE:
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)(((LPCREATESTRUCT)lParam)->lpCreateParams)); //Store the reference to the port number variable
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case m_CONTROL_ID::OK_BUTTON:
			PostMessage(hWnd, WM_CLOSE, NULL, NULL);
			break;
		case m_CONTROL_ID::CANCEL_BUTTON:
			DestroyWindow(hWnd);
			break;
		}
		break;
	case WM_CLOSE:
		BOOL bSuccess; bSuccess = FALSE;
		UINT nPort; nPort = GetDlgItemInt(hWnd, m_CONTROL_ID::PORT_EDIT, &bSuccess, FALSE);
		UINT * pnPortNum;
		pnPortNum = (UINT*)GetWindowLongPtr(hWnd, GWLP_USERDATA); //Get the pointer to the serial handler

		if (bSuccess) {
			if (SerialHandler::CheckPort(nPort)) {
				MessageBox(hWnd, L"Successfully connected to port", L"Success", NULL);
				*pnPortNum = nPort;
				DestroyWindow(hWnd);
			}
			else {
				MessageBox(hWnd, L"Failed to connected to port", L"Error", MB_ICONERROR);
			}
		}
		break;
	case WM_DESTROY:
		SendMessage(GetWindow(hWnd, GW_OWNER), WM_PARENTNOTIFY, NULL, NULL); //TODO: This should not have to be called manually
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

/*
	Register the window.

	@return The window class's atom.
*/
ATOM PortSettingsWnd::Register() CONST{
	WNDCLASSEX wc;
	wc.cbClsExtra = 0;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = m_TITLE;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	return RegisterClassEx(&wc);
}

/*
	Creates the window.

	@param hWndParent: A handle to the parent window.
	@param rnPortNum: A reference to the port number variable to use.

	@return TRUE if the window was successfully created, otherwise FALSE.
*/
BOOL PortSettingsWnd::Create(CONST HWND hWndParent, CONST UINT& rnPortNum) CONST{
	Register();

	HWND hWnd = CreateWindow(
		m_TITLE, m_TITLE,
		(WS_OVERLAPPEDWINDOW | WS_POPUPWINDOW | WS_VISIBLE) & ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX),
		CW_USEDEFAULT, CW_USEDEFAULT,
		200, 150,
		hWndParent,
		NULL,
		GetModuleHandle(NULL),
		(LPVOID)&rnPortNum);

	if (hWnd == NULL) {
		return FALSE;
	}
	
	try {
		CreateControls(hWnd, rnPortNum);
	}
	catch (LPCWSTR errorMsg) {
		//TODO: Handle error
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return TRUE;
}

/*
	Creates the controls for the window (the text, edit and buttons).

	@param hWndParent: A handle to the parent (main) window.
	@param nPortNum: The port number.
*/
VOID PortSettingsWnd::CreateControls(CONST HWND hWndParent, CONST UINT nPortNum) CONST{
	RECT clientRect; GetClientRect(hWndParent, &clientRect);

	//Port number text
	CONST auto margin = 5, textWidth = 70, textHeight = 20, textPadding = 5;
	HWND hTextPortNum = CreateWindow(WC_STATIC, L"Port number: ",
		WS_CHILD | WS_VISIBLE | SS_SUNKEN,
		margin, clientRect.bottom / 2 - textHeight / 2,
		textWidth, 20,
		hWndParent,
		NULL,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hTextPortNum, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);

	//Port number edit
	CONST auto editWidth = 50, editHeight = 22;
	HWND hEditPort = CreateWindow(
		WC_EDIT, L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER | ES_LEFT,
		margin + textWidth + textPadding, clientRect.bottom / 2 - textHeight / 2,
		editWidth, editHeight,
		hWndParent,
		(HMENU)m_CONTROL_ID::PORT_EDIT,
		GetModuleHandle(NULL),
		NULL);
	if (hEditPort == NULL) {
		//TODO: Throw something
	}
	SendMessage(hEditPort, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	SetDlgItemInt(hWndParent, m_CONTROL_ID::PORT_EDIT, nPortNum, FALSE);
	SendMessage(hEditPort, EM_SETLIMITTEXT, 4, NULL);

	//OK button
	const auto buttonWidth = 75, buttonHeight = 25, buttonMidPadding = 10;
	HWND hBtnOk = CreateWindow(L"Button", L"OK",
		WS_CHILD | WS_VISIBLE | BS_CENTER,
		clientRect.right / 2 - buttonWidth - buttonMidPadding, clientRect.bottom - buttonHeight - margin,
		buttonWidth, buttonHeight,
		hWndParent,
		(HMENU)m_CONTROL_ID::OK_BUTTON,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hBtnOk, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);

	//CANCEL button
	HWND hBtnCancel = CreateWindow(
		L"Button", L"Cancel",
		WS_CHILD | WS_VISIBLE | BS_CENTER,
		clientRect.right / 2 + buttonMidPadding, clientRect.bottom - buttonHeight - margin,
		buttonWidth, buttonHeight,
		hWndParent,
		(HMENU)m_CONTROL_ID::CANCEL_BUTTON,
		GetModuleHandle(NULL),
		NULL);
	SendMessage(hBtnCancel, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
}