/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SceneUtils.h
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
JSFunction(SceneGetRootEntities);

Meta(Enable, ExposeJavaScript)
JSFunction(SceneGetActiveEntities);

Meta(Enable, ExposeJavaScript)
JSFunction(SceneLoad);

Meta(Enable, ExposeJavaScript)
JSFunction(SceneSave);

Meta(Enable, ExposeJavaScript)
JSFunction(ScenePlay);

Meta(Enable, ExposeJavaScript)
JSFunction(SceneStep);