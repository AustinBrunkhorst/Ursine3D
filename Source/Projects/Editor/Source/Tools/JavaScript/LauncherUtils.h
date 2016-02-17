/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** LauncherUtils.h
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
JSFunction(GetEditorVersion);

Meta(Enable, ExposeJavaScript)
JSFunction(BrowseFile);

Meta(Enable, ExposeJavaScript)
JSFunction(BrowseFolder);

Meta(Enable, ExposeJavaScript)
JSFunction(LauncherProjectCreate);

Meta(Enable, ExposeJavaScript)
JSFunction(LauncherProjectOpen);