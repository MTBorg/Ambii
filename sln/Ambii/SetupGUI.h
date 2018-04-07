/*
	//TODO: Comment
*/

#ifndef SETUPGUI_H
#define SETUPGUI_H

#include <Windows.h>

/*
	//TODO: Comment
*/
class SetupGUI : public IDropTarget {
private:
	//Inherited functions
	HRESULT DragEnter( IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
	HRESULT DragLeave();
	HRESULT DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
	HRESULT Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);

public:
	HWND Create(CONST HWND hWndParent, CONST UINT x, CONST UINT y, CONST UINT width, CONST UINT height);
};

#endif