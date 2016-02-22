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
JSFunction(SceneSaveWorld);

Meta(Enable, ExposeJavaScript)
JSFunction(SceneSaveWorldAs);

Meta(Enable, ExposeJavaScript)
JSFunction(SceneSetActiveWorld);

Meta(Enable, ExposeJavaScript)
JSFunction(SceneGetRootEntities);

Meta(Enable, ExposeJavaScript)
JSFunction(SceneGetActiveEntities);

Meta(Enable, ExposeJavaScript)
JSFunction(ScenePlayStart);

Meta(Enable, ExposeJavaScript)
JSFunction(SceneSetPlayState);

Meta(Enable, ExposeJavaScript)
JSFunction(SceneStep);

Meta(Enable, ExposeJavaScript)
JSFunction(ScenePlayStop);

Meta(Enable, ExposeJavaScript)
JSFunction(SceneGetEntitySystems);