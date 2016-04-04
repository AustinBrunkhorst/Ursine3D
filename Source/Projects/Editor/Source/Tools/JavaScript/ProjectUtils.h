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
JSFunction(ProjectSave);

Meta(Enable, ExposeJavaScript)
JSFunction(ProjectBuildStart);

Meta(Enable, ExposeJavaScript)
JSFunction(ProjectSetFullScreen);

Meta(Enable, ExposeJavaScript)
JSFunction(ProjectGetEditorPreferences);

Meta(Enable, ExposeJavaScript)
JSFunction(ProjectGetName);

Meta(Enable, ExposeJavaScript)
JSFunction(ProjectGetResourceTree);

Meta(Enable, ExposeJavaScript)
JSFunction(ProjectGetResourcesByType);

Meta(Enable, ExposeJavaScript)
JSFunction(ProjectGetResource);

Meta(Enable, ExposeJavaScript)
JSFunction(ProjectDeleteResource);

Meta(Enable, ExposeJavaScript)
JSFunction(ProjectRenameResource);

Meta(Enable, ExposeJavaScript)
JSFunction(ProjectMakeEditorResource);

Meta(Enable, ExposeJavaScript)
JSFunction(ProjectSetEmptyScene);

Meta(Enable, ExposeJavaScript)
JSFunction(ProjectOpenNew);