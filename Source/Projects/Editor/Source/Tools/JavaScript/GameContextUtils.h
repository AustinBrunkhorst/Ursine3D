/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GameContextUtils.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <NativeJSFunction.h>

Meta(Enable, ExposeJavaScript)
JSFunction(GameContextGetWindowFullScreen);

Meta(Enable, ExposeJavaScript)
JSFunction(GameContextSetWindowFullScreen);

Meta(Enable, ExposeJavaScript)
JSFunction(GameSetVolume);

Meta(Enable, ExposeJavaScript)
JSFunction(GameContextExitGame);

Meta(Enable, ExposeJavaScript)
JSFunction(GameContextGetAvailableManagers);
