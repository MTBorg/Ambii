/*
	RegistryHandler.h

	Header file for class RegistryHandler

	@author Martin Terneborg
*/

#ifndef REGISTRYHANDLER_H
#define REGISTRYHANDLER_H

#include <Windows.h>

#include "Monitor.h"
#include "Settings.h"

/*
	Class responsible for handling all operations to/from
	the registry where the application's settings are stored.
*/
class RegistryHandler {
private:
	static CONST LPCWSTR m_AppKeyName;
	static CONST struct m_valueNames {
		static CONST LPCWSTR REFRESHRATE;
		static CONST LPCWSTR SAMPLESIZE;
		static CONST LPCWSTR MULTITHREADING;
		static CONST LPCWSTR USEONLYPRIMARY;
		static CONST LPCWSTR LEDS_LEFT;
		static CONST LPCWSTR LEDS_RIGHT;
		static CONST LPCWSTR LEDS_TOP;
		static CONST LPCWSTR LEDS_BOTTOM;
		static CONST LPCWSTR NAME_DISPLAYDEVICE;
		static CONST LPCWSTR NAME_MONITOR;
		static CONST LPCWSTR POSITION_HORZ;
		static CONST LPCWSTR POSITION_VERT;
		static CONST LPCWSTR POSITION_LEFT;
		static CONST LPCWSTR POSITION_RIGHT;
		static CONST LPCWSTR POSITION_TOP;
		static CONST LPCWSTR POSITION_BOTTOM;
		static CONST LPCWSTR PORTNUM;
		static CONST LPCWSTR BAUDRATE;
		static CONST LPCWSTR CLOCKWISE;
	};

	static Monitor GetMonitor(CONST HKEY hKeyApp, CONST LPCWSTR lpwstrMonitor);
	static DWORD GetValueDWord(CONST HKEY hKey, CONST LPCWSTR subKey,
		CONST LPCWSTR valueName);
	static std::wstring GetValueString(CONST HKEY hKey, CONST LPCWSTR subKey,
		CONST LPCWSTR valueName);
	static BOOL GetValueBool(CONST HKEY hKey, CONST LPCWSTR subKey,
		CONST LPCWSTR valueName);

	static VOID StoreMonitor(CONST HKEY hKeyApp, CONST Monitor& monitor);
	static VOID StoreValueDWord(CONST HKEY hKey, CONST LPCWSTR valueName,
		CONST DWORD dwValue);
	static VOID StoreValueString(CONST HKEY hKey, CONST LPCWSTR valueName,
		CONST LPCWSTR valueStr, CONST DWORD stringLength);
	static VOID StoreValueBool(CONST HKEY hKey, CONST LPCWSTR valueName,
		CONST BOOL bValue);
public:
	static BOOL GetSettings(Settings& settings);
	static VOID StoreSettings( Settings& settings);
};

#endif