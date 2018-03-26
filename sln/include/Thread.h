/*
	Thread.h

	Header file for clas Thread.

	@author: Martin Terneborg.
*/

#ifndef THREAD_H
#define THREAD_H

#include <Windows.h>

/*
	Abstract class for Threads. A class can inherit this class and only define the Run() function
	and let this class handle the rest of the thread management.
 */
class Thread{
private:
	virtual VOID Run() = 0;
	static UINT __stdcall StaticEntryPoint(VOID * CONST pThis);

public:
	HANDLE Create();
};

#endif