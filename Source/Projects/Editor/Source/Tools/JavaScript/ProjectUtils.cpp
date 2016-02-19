/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ProjectUtils.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ProjectUtils.h"
#include "Editor.h"

JSFunction(ProjectGetName)
{
    auto *editor = GetCoreSystem( Editor );

	return CefV8Value::CreateString( editor->GetProject( ).GetConfig( ).title );
}