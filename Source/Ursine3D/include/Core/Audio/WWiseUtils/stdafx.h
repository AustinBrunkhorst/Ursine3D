//////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006 Audiokinetic Inc. / All Rights Reserved
//
//////////////////////////////////////////////////////////////////////

#pragma once

#ifdef AK_WIN
	#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
	#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
	#endif

	#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
	// Windows Header Files:
	#include <windows.h>
#endif

// TODO: reference additional headers your program requires here
