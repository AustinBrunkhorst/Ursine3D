/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntityUtils.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "EntityUtils.h"

#include <Editor.h>

#include <World.h>

using namespace ursine;
using namespace std::placeholders;

JSFunction(CreateEntity)
{
    auto *world = GetCoreSystem( Editor )->GetProject( )->GetScene( ).GetActiveWorld( );

    auto *entity = world->CreateEntity( );

    return CefV8Value::CreateUInt( entity->GetUniqueID( ) );
}