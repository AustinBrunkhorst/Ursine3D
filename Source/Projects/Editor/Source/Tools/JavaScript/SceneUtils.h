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