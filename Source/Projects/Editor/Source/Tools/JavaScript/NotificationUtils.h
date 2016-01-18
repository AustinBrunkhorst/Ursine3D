/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DebugUtils.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <NativeJSFunction.h>

namespace notification
{
    Meta(Enable, ExposeJavaScript)
    JSFunction(NotificationButtonCallback);

    Meta(Enable, ExposeJavaScript)
    JSFunction(NotificationCloseCallback);
}
