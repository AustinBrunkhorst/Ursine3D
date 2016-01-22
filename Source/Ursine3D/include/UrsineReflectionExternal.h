/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UrsineReflectionExternal.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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
