/*
	//TODO: Comment
*/

#ifndef INITCTRLS_H
#define INITCTRLS_H

#include <Windows.h>

CONST HWND InitTextCtrl(CONST HWND hWndParent, CONST LPCWSTR text, CONST UINT x, CONST UINT y);
CONST HWND InitEditCtrl(CONST HWND hWndParent, CONST UINT x, CONST UINT y, CONST UINT width, CONST UINT height, CONST HMENU id);
CONST HWND InitCheckboxCtrl(CONST HWND hWndParent, CONST UINT x, CONST UINT y, CONST LPCWSTR text, CONST HMENU id);

#endif