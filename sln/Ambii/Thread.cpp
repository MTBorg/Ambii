/*
	Thread.cpp

	Source file for clas Thread.

	@author: Martin Terneborg.
*/

#include "Thread.h"

#include <process.h>

/*
	The static entry point function which calls the derived Run() function.
	This is called by the function _beginthreadex() in function Thread::Create().

	@param pThis: The "this" pointer for the object that calls this function.

	@return Always returns 1
*/
UINT __stdcall Thread::StaticEntryPoint(VOID * CONST pThis) {
	((Thread*)pThis)->Run();
	return 1;
} 

/*
	Creates the thread.

	@return A handle to the thread created or NULL if the call failed.
*/
HANDLE Thread::Create(){
	return (HANDLE)_beginthreadex(NULL, 0, StaticEntryPoint, this, 0, 0);
}