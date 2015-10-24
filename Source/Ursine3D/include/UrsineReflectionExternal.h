#pragma once

#include "Meta.h"

///////////////////////////////////////////////////////////////////////////////
// Chromium Embedded Framework
///////////////////////////////////////////////////////////////////////////////

#if defined(URSINE_DEPENDENCY_CEF)

#include <cef_v8.h>

MetaExternal( CefString );
MetaExternal( CefRefPtr<CefString> );
MetaExternal( CefRefPtr<CefV8Value> );
MetaExternal( CefV8ValueList );

#endif
