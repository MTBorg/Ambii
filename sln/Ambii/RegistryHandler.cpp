/*
	RegistryHandler.cpp

	Source file for class RegisterHandler

	@author Martin Terneborg
*/

#include "RegistryHandler.h"

#include <string>

using std::wstring;

CONST LPCWSTR RegistryHandler::m_AppKeyName = L"Ambii";

CONST LPCWSTR RegistryHandler::m_valueNames::LEDS_LEFT = L"Leds left";
CONST LPCWSTR RegistryHandler::m_valueNames::LEDS_RIGHT = L"Leds right";
CONST LPCWSTR RegistryHandler::m_valueNames::LEDS_TOP = L"Leds top";
CONST LPCWSTR RegistryHandler::m_valueNames::LEDS_BOTTOM = L"Leds bottom";
CONST LPCWSTR RegistryHandler::m_valueNames::NAME_MONITOR = L"Monitor name";
CONST LPCWSTR RegistryHandler::m_valueNames::NAME_DISPLAYDEVICE = L"Display device name";
CONST LPCWSTR RegistryHandler::m_valueNames::REFRESHRATE = L"Maximum refresh rate";
CONST LPCWSTR RegistryHandler::m_valueNames::SAMPLESIZE = L"Sample size";
CONST LPCWSTR RegistryHandler::m_valueNames::POSITION_HORZ = L"Horizontal Position";
CONST LPCWSTR RegistryHandler::m_valueNames::POSITION_VERT = L"Vertical Position";
CONST LPCWSTR RegistryHandler::m_valueNames::POSITION_LEFT = L"Left side position";
CONST LPCWSTR RegistryHandler::m_valueNames::POSITION_RIGHT = L"Right side Position";
CONST LPCWSTR RegistryHandler::m_valueNames::POSITION_TOP = L"Top side position";
CONST LPCWSTR RegistryHandler::m_valueNames::POSITION_BOTTOM = L"Bottom side position";
CONST LPCWSTR RegistryHandler::m_valueNames::MULTITHREADING = L"Multithreading";
CONST LPCWSTR RegistryHandler::m_valueNames::USEONLYPRIMARY = L"Use only primary monitor";
CONST LPCWSTR RegistryHandler::m_valueNames::PORTNUM = L"Port number";
CONST LPCWSTR RegistryHandler::m_valueNames::BAUDRATE = L"Baud Rate";
CONST LPCWSTR RegistryHandler::m_valueNames::CLOCKWISE = L"Clockwise";
CONST LPCWSTR RegistryHandler::m_valueNames::CLOCKWISE_LEFT = L"Left side clockwise";
CONST LPCWSTR RegistryHandler::m_valueNames::CLOCKWISE_RIGHT = L"Right side clockwise";
CONST LPCWSTR RegistryHandler::m_valueNames::CLOCKWISE_TOP = L"Top side clockwise";
CONST LPCWSTR RegistryHandler::m_valueNames::CLOCKWISE_BOTTOM = L"Bottom side clockwise";


/*
	Stores the monitor specified by parameter monitor in the registry.

	@param hKeyApp: A handle to the application-key where the monitor key will be stored.
	@param monitor: A reference to the monitor object which will be stored.

	@throw A std::wstring containing the error message.
*/
VOID RegistryHandler::StoreMonitor(CONST HKEY hKeyApp, CONST Monitor& monitor) {
	INT i = 0;
	WCHAR buffer[3];
	DWORD disposition;
	HKEY hKeyMonitor;
	wstring keyName(monitor.GetMonitorName());
	try {
		do {
			//Find out how many digits there are in the monitor number  i
			UINT8 n = 0;
			UINT8 temp = i;
			do {
				temp -= temp % 10;
				temp /= 10;
				n++;
			} while (temp > 0);

			//Append a number at the end of the monitor's key name
			//if a monitor with the same name exists
			WCHAR * buf = new WCHAR[n + 2]; //Digit + null terminator + sign character.
			if (i > 0) {
				_itow(i, buf, 10);
				keyName.append(L" (");
				keyName.append(buf);
				keyName.append(L")");
			}
			delete[] buf;

			if (RegCreateKeyEx(hKeyApp, keyName.c_str(), NULL, NULL,
				REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKeyMonitor, &disposition) != ERROR_SUCCESS) {
				throw wstring(L"Failed to create key for monitor: """) + wstring(monitor.GetMonitorName());
				//TODO: Handle error with FormatMessage()
			}
			i++;
		} while (disposition == REG_OPENED_EXISTING_KEY); 

		//Store the values
		StoreValueString(hKeyMonitor, m_valueNames::NAME_MONITOR, monitor.GetMonitorName().c_str(),
			monitor.GetMonitorName().length());
		StoreValueString(hKeyMonitor, m_valueNames::NAME_DISPLAYDEVICE,
			monitor.GetDisplayDeviceName().c_str(), monitor.GetDisplayDeviceName().length());
		
		StoreValueDWord(hKeyMonitor, m_valueNames::LEDS_LEFT, monitor.GetLeftLeds());
		StoreValueDWord(hKeyMonitor, m_valueNames::LEDS_RIGHT, monitor.GetRightLeds());
		StoreValueDWord(hKeyMonitor, m_valueNames::LEDS_TOP, monitor.GetTopLeds());
		StoreValueDWord(hKeyMonitor, m_valueNames::LEDS_BOTTOM, monitor.GetBottomLeds());
		StoreValueDWord(hKeyMonitor, m_valueNames::POSITION_HORZ, monitor.GetPosX());
		StoreValueDWord(hKeyMonitor, m_valueNames::POSITION_VERT, monitor.GetPosY());
		StoreValueDWord(hKeyMonitor, m_valueNames::POSITION_LEFT, monitor.GetPosLeft());
		StoreValueDWord(hKeyMonitor, m_valueNames::POSITION_RIGHT, monitor.GetPosRight());
		StoreValueDWord(hKeyMonitor, m_valueNames::POSITION_TOP, monitor.GetPosTop());
		StoreValueDWord(hKeyMonitor, m_valueNames::POSITION_BOTTOM, monitor.GetPosBottom());

		StoreValueBool(hKeyMonitor, m_valueNames::CLOCKWISE_LEFT, monitor.GetClockwiseLeft());
		StoreValueBool(hKeyMonitor, m_valueNames::CLOCKWISE_RIGHT, monitor.GetClockwiseRight());
		StoreValueBool(hKeyMonitor, m_valueNames::CLOCKWISE_TOP, monitor.GetClockwiseTop());
		StoreValueBool(hKeyMonitor, m_valueNames::CLOCKWISE_BOTTOM, monitor.GetClockwiseBottom());


		//Close retrieved handle
		RegCloseKey(hKeyMonitor);
	}
	catch (wstring errorMsg) {
		throw wstring(L"Exception in function StoreMonitor. Exception: \n") + errorMsg;
	}
}

/*
	Stores the settings passed to the function in the registry.

	@param settings, a reference to the settings object to store.

	@throw A string containing the error message.
*/
VOID RegistryHandler::StoreSettings(Settings& settings) {
	HKEY hKeySoftware = NULL;
	HKEY hKeyApp = NULL;

	//Store the values
	try {
		//Retrieve the software-key (HKEY_CURRENT_USER\SOFTWARE)
		if (RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE", NULL,
			KEY_WRITE, &hKeySoftware) != ERROR_SUCCESS) {
			throw wstring(L"Failed to retrieve key to HKEY_CURRENT_USER\\SOFTWARE");
		}

		RegDeleteTree(hKeySoftware, m_AppKeyName); //Clear the tree before recreating it

		//Create the application-key
		if (RegCreateKeyEx(hKeySoftware, m_AppKeyName, NULL, NULL, REG_OPTION_NON_VOLATILE,
			KEY_READ | KEY_WRITE, NULL, &hKeyApp, NULL) != ERROR_SUCCESS) {
			throw wstring(L"Failed to create application-key");
		}

		StoreValueDWord(hKeyApp, m_valueNames::REFRESHRATE, settings.m_maxRefreshRate);
		StoreValueDWord(hKeyApp, m_valueNames::SAMPLESIZE, settings.m_sampleSize);
		StoreValueDWord(hKeyApp, m_valueNames::PORTNUM, settings.m_portNum);
		StoreValueDWord(hKeyApp, m_valueNames::BAUDRATE, settings.m_nBaudRate);

		StoreValueBool(hKeyApp, m_valueNames::MULTITHREADING, settings.m_bMultiThreading);
		StoreValueBool(hKeyApp, m_valueNames::USEONLYPRIMARY, settings.m_bUsePrimaryMonitor);
		StoreValueBool(hKeyApp, m_valueNames::CLOCKWISE, settings.m_bClockwise);

		for (UINT i = 0; i < settings.m_usedMonitors.size(); i++) {
			StoreMonitor(hKeyApp, settings.m_usedMonitors.at(i));
		}

		//Close retrieved handle
		RegCloseKey(hKeySoftware);
		RegCloseKey(hKeyApp);
	}
	catch (wstring errorMsg) {
		//Close retrieved handle
		RegCloseKey(hKeySoftware);
		RegCloseKey(hKeyApp);

		throw wstring(L"Exception in function StoreSettings. Exception: \n") + errorMsg;
	}
}

/*
	Gets the settings saved in the registry.

	@param settings, A reference to the settings object to save to.

	@throw A string containing the error message.

	@return TRUE if settings were retrieved successfully, otherwise FALSE.
*/
BOOL RegistryHandler::GetSettings(Settings& settings) {
	//Get the size of the subkey with the longest name
	//TODO: This doesn't work, seems to only return the length of the longest
	//		name that has ever existed in the key
	/*DWORD maxLength;
	if (RegQueryInfoKey(hKeyApp, NULL, NULL, NULL, NULL,
		&maxLength, NULL, NULL, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) {
		//TODO: Handle error with FormatMessage();
	}*/

	HKEY hKeySoftware = NULL;
	HKEY hKeyApp = NULL;

	try {
		if (RegOpenKey(HKEY_CURRENT_USER, L"SOFTWARE", &hKeySoftware)) {
			throw wstring(L"Failed to open key \"HKEY_CURRENT_USER\\SOFTWARE\"");
			//TODO: Handle error with FormatMessage();
		}

		if (RegOpenKey(hKeySoftware, m_AppKeyName, &hKeyApp) != ERROR_SUCCESS) {
			throw wstring(L"Failed to open key \"HKEY_CURRENT_USER\\SOFTWARE\\\"") + wstring(m_AppKeyName);
			//TODO: Handle error with FormatMessage();
		}

		//Get the maximum refresh rate and sample size
		DWORD refreshRate = GetValueDWord(hKeyApp, NULL, m_valueNames::REFRESHRATE);
		DWORD sampleSize = GetValueDWord(hKeyApp, NULL, m_valueNames::SAMPLESIZE);
		DWORD portNum = GetValueDWord(hKeyApp, NULL, m_valueNames::PORTNUM);
		DWORD baudRate = GetValueDWord(hKeyApp, NULL, m_valueNames::BAUDRATE);

		BOOL bMultiThreading = GetValueBool(hKeyApp, NULL, m_valueNames::MULTITHREADING);
		BOOL bUseOnlyPrimary = GetValueBool(hKeyApp, NULL, m_valueNames::USEONLYPRIMARY);
		BOOL bClockwise = GetValueBool(hKeyApp, NULL, m_valueNames::CLOCKWISE);

		//Enumerate the subkeys (monitors)
		//TODO: This should not use MAX_PATH
		INT i = 0;
		WCHAR keyName[MAX_PATH];
		std::vector<Monitor> monitors;
		while (RegEnumKey(hKeyApp, i, keyName, MAX_PATH) != ERROR_NO_MORE_ITEMS) {
			monitors.push_back(GetMonitor(hKeyApp, keyName));
			i++;
		}

		//Get the monitors
		std::vector<Monitor> allMonitors = Monitor::GetMonitors();
		settings.m_usedMonitors.clear();
		for (UINT i = 0; i < monitors.size(); i++) {
			BOOL connected = FALSE;
			for (UINT j = 0; j < allMonitors.size(); j++) {
				if (monitors.at(i).GetDisplayDeviceName() == allMonitors.at(j).GetDisplayDeviceName()) {
					monitors.at(i).SetMonitorHandle(allMonitors.at(j).GetMonitorHandle());
					settings.m_usedMonitors.push_back(monitors.at(i));
					connected = TRUE;
					break;
				}
			}
			if (!connected) {//Monitor is saved but couldn't be found (i.e. not connected)
				return FALSE;
			}
		}

		settings.m_sampleSize = sampleSize;
		settings.m_maxRefreshRate = (UINT8)refreshRate;
		settings.m_bMultiThreading = bMultiThreading;
		settings.m_bUsePrimaryMonitor = bUseOnlyPrimary;
		settings.m_portNum = portNum;
		settings.m_nBaudRate = baudRate;
		settings.m_bDisplayMonitors = FALSE;
		settings.m_bDisplayOutput = FALSE;
		settings.m_bClockwise = bClockwise;
	}
	catch (std::wstring errorMsg) {
		//Close retrieved handles
		RegCloseKey(hKeySoftware);
		RegCloseKey(hKeyApp);

		throw wstring(L"Exception in function GetSettings. Exception: \n") + errorMsg;
	}

	//Close retrieved handles
	RegCloseKey(hKeySoftware);
	RegCloseKey(hKeyApp);

	return TRUE;
}

/*
	Returns a monitor object with values (not including the monitor handle) stored at
	the key which names matches parameter monitorKeyName.

	@param hKeyApp, A handle to the application-key.
	@param monitorKeyName, The name of the key representing the monitor.
	
	@throw A string containing the error message.

	@return A monitor object with values of corresponding monitor key.
*/
Monitor RegistryHandler::GetMonitor(CONST HKEY hKeyApp, CONST LPCWSTR monitorKeyName) {
	Monitor monitor;

	try {
		//Get the monitor name and display device name
		monitor.SetDisplayDeviceName(GetValueString(hKeyApp, monitorKeyName, m_valueNames::NAME_DISPLAYDEVICE).c_str());
		monitor.SetMonitorName(GetValueString(hKeyApp, monitorKeyName, m_valueNames::NAME_MONITOR).c_str());

		//Get the LED-settings
		monitor.SetLeftLeds(GetValueDWord(hKeyApp, monitorKeyName, m_valueNames::LEDS_LEFT));
		monitor.SetRightLeds(GetValueDWord(hKeyApp, monitorKeyName, m_valueNames::LEDS_RIGHT));
		monitor.SetTopLeds(GetValueDWord(hKeyApp, monitorKeyName, m_valueNames::LEDS_TOP));
		monitor.SetBottomLeds(GetValueDWord(hKeyApp, monitorKeyName, m_valueNames::LEDS_BOTTOM));

		//Get the positions
		monitor.SetPosX((UINT8)GetValueDWord(hKeyApp, monitorKeyName, m_valueNames::POSITION_HORZ));
		monitor.SetPosY((UINT8)GetValueDWord(hKeyApp, monitorKeyName, m_valueNames::POSITION_VERT));
		monitor.SetPosLeft((UINT8)GetValueDWord(hKeyApp, monitorKeyName, m_valueNames::POSITION_LEFT));
		monitor.SetPosRight((UINT8)GetValueDWord(hKeyApp, monitorKeyName, m_valueNames::POSITION_RIGHT));
		monitor.SetPosTop((UINT8)GetValueDWord(hKeyApp, monitorKeyName, m_valueNames::POSITION_TOP));
		monitor.SetPosBottom((UINT8)GetValueDWord(hKeyApp, monitorKeyName, m_valueNames::POSITION_BOTTOM));

		//Get clockwise-booleans
		monitor.SetClockwiseLeft(GetValueBool(hKeyApp, monitorKeyName, m_valueNames::CLOCKWISE_LEFT));
		monitor.SetClockwiseTop(GetValueBool(hKeyApp, monitorKeyName, m_valueNames::CLOCKWISE_RIGHT));
		monitor.SetClockwiseRight(GetValueBool(hKeyApp, monitorKeyName, m_valueNames::CLOCKWISE_TOP));
		monitor.SetClockwiseBottom(GetValueBool(hKeyApp, monitorKeyName, m_valueNames::CLOCKWISE_BOTTOM));

	}
	catch (std::wstring errorMsg) {
		throw wstring(L"Exception in function GetMonitor. Exception: \n") + errorMsg;
	}

	return monitor;
}


/*
	Returns the DWORD value with the specified name in the specified key. If the value is not
	successfully retrieved (i.e. the value or the keys do not exist) an exception is thrown.

	@param hKey, A handle to the key containing the subkey in which the value exists.
	@param subKey, A string containing the name of the subkey in which the value exists.
	@param valueName, A string containing the name of the value to get.

	@throw A string containing the error message.

	@return The specified value.
*/
DWORD RegistryHandler::GetValueDWord(CONST HKEY hKey, CONST LPCWSTR subKey, CONST LPCWSTR valueName) {
	DWORD value;
	DWORD size = sizeof(DWORD);
	if (RegGetValue(hKey, subKey, valueName, RRF_RT_REG_DWORD, NULL, (LPVOID)&value, &size) != ERROR_SUCCESS) {
		throw wstring(L"Exception in function GetValueDWord. Exception: Failed to retrieve value \"") +
			std::wstring(valueName) + wstring(L"\" from registry.");
	}
	return value;
}


/*
	Returns the string value with the specified name in the specified key. If the value is not
	successfully retrieved (i.e. the value or the keys do not exist) an exception is thrown.

	@param hKey, A handle to the key containing the subkey in which the value exists.
	@param subKey, A string containing the name of the subkey in which the value exists.
	@param valueName, A string containing the name of the value to get.

	@throw A string containing the error message.

	@return The specified value.
*/
std::wstring RegistryHandler::GetValueString(CONST HKEY hKey, CONST LPCWSTR subKey, CONST LPCWSTR valueName) {
	//TODO: This shouldn't use MAX_PATH
	WCHAR buffer[MAX_PATH];
	CONST DWORD size = MAX_PATH;
	if (RegGetValue(hKey, subKey, valueName, RRF_RT_REG_SZ, NULL,
	(LPVOID)&buffer, (LPDWORD)&size) != ERROR_SUCCESS) {
		throw wstring(L"Exception in function GetValueString. Exception: Failed to retrieve value \"") +
			wstring(valueName) + wstring(L"\" from registry.");
	}
	return wstring(buffer);
}

/*
	Returns the boolean value with the specified name in the specified key. If the value is not
	successfully retrieved (i.e. the value or the keys do not exist) an exception is thrown.

	@param hKey: A handle to the key containing the subkey in which the value exists.
	@param subKey: A string containing the name of the subkey in which the value exists.
	@param valueName: A string containing the name of the value to get.

	@throw A string containing the error message.

	@return The specified value.
*/
BOOL RegistryHandler::GetValueBool(CONST HKEY hKey, CONST LPCWSTR subKey,
	CONST LPCWSTR valueName) {
	BOOL bResult;
	DWORD dwSize = sizeof(BOOL);
	if (RegGetValue(hKey, subKey, valueName, RRF_RT_REG_BINARY, NULL, &bResult, &dwSize) != ERROR_SUCCESS) {
		throw wstring(L"Exception in function GetValueBool. Exception: Failed to retrieve value \"") +
			wstring(valueName) + wstring(L"\" from registry.");
	}
	return bResult;
}

/*
	Stores the specified DWORD value in the registry.

	@param hKey: A handle to the key in which to store the value.
	@param valueName: The registry name of the value to store
	@param value: The DWORD value to store.

	@throw A string containing the error message.
*/
VOID RegistryHandler::StoreValueDWord(CONST HKEY hKey, CONST LPCWSTR valueName,
	CONST DWORD dwValue) {
	if (RegSetValueEx(hKey, valueName, NULL, REG_DWORD,
		(BYTE*)&dwValue, sizeof(DWORD)) != ERROR_SUCCESS) {
		throw wstring(L"Exception in function StoreValueDWord. Exception: Failed to store value \"") +
			wstring(valueName) + wstring(L"\"");
	}
}

/*
	Stores the specified string value in the registry.

	@param hKey: A handle to the key in which to store the value.
	@param valueName: The registry name of the value to store
	@param value: The string to store.
	@param stringLength: 

	@throw A std::wstring containing the error message.
*/
VOID RegistryHandler::StoreValueString(CONST HKEY hKey, CONST LPCWSTR valueName,
	CONST LPCWSTR valueStr, CONST DWORD stringLength) {
	if (RegSetValueEx(hKey, valueName, NULL, REG_SZ,
		(BYTE*)valueStr, sizeof(WCHAR) * stringLength) != ERROR_SUCCESS) {
		throw wstring(L"Exception in function StoreValueString. Exception: Failed to store value \"") +
			wstring(valueName) + wstring(L"\"");
	}
}

/*
	Stores the specified boolean value in the registry.

	@param hKey: A handle to the key in which to store the value.
	@param valueName: The registry name of the value to store.
	@param bValue: The boolean value to store.

	@throw A std::wstring containing the error message.
*/
VOID RegistryHandler::StoreValueBool(CONST HKEY hKey, CONST LPCWSTR valueName,
	CONST BOOL bValue) {
	if (RegSetValueEx(hKey, valueName, NULL, REG_BINARY, (BYTE*)&bValue, sizeof(BOOL)) != ERROR_SUCCESS) {
		throw wstring(L"Exception in function StoreValueS. Exception: Failed to store value \"") +
			wstring(valueName) + wstring(L"\"");
	}
}