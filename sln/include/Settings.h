/*
	Settings.h

	Header file for class Settings.

	@author Martin Terneborg.
*/

#ifndef SETTINGS_H
#define SETTINGS_H

#include <vector>

#include "Monitor.h"

/*
	Holds all settings-values and provides settings related functions
*/
class Settings {
public:
	UINT8 m_maxRefreshRate;
	UINT m_sampleSize;
	BOOL m_bMultiThreading, m_bUsePrimaryMonitor;
	BOOL m_bDisplayMonitors = FALSE, m_bDisplayOutput = FALSE;
	BOOL m_bClockwise = TRUE;
	UINT m_portNum;
	UINT m_nBaudRate;
	std::vector<Monitor> m_usedMonitors;

};

#endif