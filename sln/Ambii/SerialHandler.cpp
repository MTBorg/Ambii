/*
	SerialHandler.cpp

	Source file for class SerialHandler.

	@author: Martin Terneborg.
*/

#include "SerialHandler.h"

#include <string>

BOOL SerialHandler::m_bStopped = FALSE;

/*
	//TODO: Comment
*/
VOID SerialHandler::Reconnect() {
	m_bStopped = FALSE;
}

/*
	//TODO: Comment
*/
BOOL SerialHandler::SetBaudRate(CONST UINT nPortNum, CONST UINT nBaudRate) {
	HANDLE hPort = OpenPort(nPortNum);
	if (hPort) {
		DCB dcb;
		GetCommState(hPort, &dcb);
		dcb.BaudRate = nBaudRate;
		SetCommState(hPort, &dcb);
		CloseHandle(hPort);
	}
	else {
		return TRUE;
	}
}

/*
	Writes the given buffer to the specified COM port.

	@param buffer: The buffer of bytes to be written.
	@param cbBufferSize: The size of the buffer in bytes.
	@param UINT nPortNum: The COM port to write to.

	@return The number of bytes written.
*/
DWORD SerialHandler::WriteToPort(CONST BYTE * CONST buffer, CONST UINT cbBufferSize, CONST UINT nPortNum) {
	HANDLE hPort = SerialHandler::OpenPort(nPortNum);
	if (!hPort) {
		MessageBox(NULL,
			L"Failed to open port while writing.",
			L"Error", MB_ICONERROR);
		m_bStopped = TRUE;
		return 0;
	}

	DCB dcb = { 0 };
	dcb.DCBlength = sizeof(dcb);
	dcb.BaudRate = CBR_57600; //TODO: This should use settings instead
	dcb.ByteSize = 8;
	dcb.StopBits = ONESTOPBIT; //TODO: This might cause problems
	dcb.Parity = NOPARITY;
	SetCommState(hPort, &dcb);

	DWORD bytesWritten = 0;

	if (!WriteFile(hPort, buffer, cbBufferSize, &bytesWritten, NULL)) {
		m_bStopped = TRUE;
	}

	CloseHandle(hPort);

	return bytesWritten;
}

/*
	Checks if the specified COM port is available.

	@param nPortNum: The COM port number to check.

	@return TRUE if available, otherwise FALSE.
*/
BOOL SerialHandler::CheckPort(CONST UINT nPortNum) {
	HANDLE hPort = OpenPort(nPortNum);
	if (hPort) {
		CloseHandle(hPort);
		return TRUE;
	}
	else {
		return FALSE;
	}
}

/*
	Opens the specified COM port.
	
	@param nPortNum: The number of the COM port to open (be sure to close the port when finished).
	//TODO: Maybe change to using non-overlapped communication

	@return The handle to opened port.
*/
HANDLE SerialHandler::OpenPort(CONST UINT nPortNum) {
	//Find the amount of digits in nPortNum
	UINT8 nLength = 0;
	UINT temp = nPortNum;
	do {
		nLength++;
	} while (temp = (temp - temp % 10) / 10); //Remove the last digit of temp

	//Create the port name
	TCHAR * portName = new TCHAR[8 + nLength]; // \\.\COMx + null terminator
	portName[7 + nLength] = '\0';
	wsprintf(portName, L"\\\\.\\COM%u\0", nPortNum);

	//Open the port
	HANDLE hPort = CreateFile(portName,
		GENERIC_WRITE,
		NULL,
		NULL,
		OPEN_EXISTING,
		NULL,
		NULL);

	delete[] portName;

	return hPort != INVALID_HANDLE_VALUE ? hPort : NULL;
}