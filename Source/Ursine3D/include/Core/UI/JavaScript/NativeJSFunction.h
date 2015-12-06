/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** NativeJSFunction.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "JSConfig.h"

#define JSFunction(name)                      \
    CefRefPtr<CefV8Value> name(JSHandlerArgs) \

/** @brief Exposes a global or static function to JavaScript.
*/
struct ExposeJavaScript : ursine::meta::MetaProperty
{
    META_OBJECT
};