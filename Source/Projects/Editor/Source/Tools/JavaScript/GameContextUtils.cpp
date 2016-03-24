/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DebugUtils.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "GameContextUtils.h"

#include "Editor.h"

JSFunction(GameContextExitGame)
{
    GetCoreSystem( Editor )->GetProject( )->GetScene( ).GetGameContext( )->ExitGame( );

    return CefV8Value::CreateBool( true );
}