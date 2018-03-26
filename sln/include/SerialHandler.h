/*
	SerialHandler.h

	Header file for class SerialHandler.

	@author: Martin Terneborg.
*/

#ifndef SERIAL_HANDLER_H
#define SERIAL_HANDLER_H

#include <Windows.h>

/*
	Class responsible for handling all serial port (COM ports) communication.
*/
class SerialHandler {
	static BOOL m_bStopped;
public:
	static BOOL CheckPort(CONST UINT nPortNum);
	static DWORD WriteToPort(CONST BYTE * CONST buffer, CONST UINT cbBufferSize, CONST UINT nPortNum);
	static HANDLE OpenPort(CONST UINT nPortNum);
	static BOOL SetBaudRate(CONST UINT nPortNum, CONST UINT nBaudRate);
	static VOID Reconnect();
	static inline BOOL Stopped() { return m_bStopped; };
};

#endif