/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ProjectUtils.h
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
JSFunction(ProjectGetName);

Meta(Enable, ExposeJavaScript)
JSFunction(ProjectGetResourceTree);

Meta(Enable, ExposeJavaScript)
JSFunction(ProjectGetResourcesByType);

Meta(Enable, ExposeJavaScript)
JSFunction(ProjectGetResource);

Meta(Enable, ExposeJavaScript)
JSFunction(ProjectSetEmptyScene);