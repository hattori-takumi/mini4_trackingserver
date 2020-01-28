// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER				0x0501
#define _WIN32_WINNT		0x0501
#define _WIN32_IE			0x0600
#define _RICHEDIT_VER		0x0500

#include <assert.h>

// ATL
#include <atlbase.h>
#include <atlwin.h>
#include <atltypes.h>

// WTL
#include <atlapp.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlcrack.h>

extern CAppModule _Module;

// M4SDK
#include <M4Tracking.h>
#pragma comment(lib, "M4Tracking.lib")

// App
#include "Resource.h"
