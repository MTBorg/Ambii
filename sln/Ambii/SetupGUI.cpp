/*
	//TODO: Comment
*/

#include <CommCtrl.h>

#include "SetupGUI.h"

/*
	//TODO: Comment
*/
HWND SetupGUI::Create(CONST HWND hWndParent, CONST UINT x, CONST UINT y, CONST UINT width, CONST UINT height,
	CONST std::vector<Monitor>& selectedMonitors) {
	HWND hStatic = CreateWindow(
		WC_STATIC, L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		x, y,
		width, height,
		hWndParent,
		NULL,
		GetModuleHandle(NULL),
		NULL);

	if (!CreateMonitors(hWndParent, selectedMonitors)) {
		throw L"Exception in function SetupGUI::Create. Failed to initialize monitors.";
	}
}

/*
	//TODO: Comment
*/
BOOL SetupGUI::CreateMonitors(CONST HWND hWndParent, CONST std::vector<Monitor>& selectedMonitors) {

}