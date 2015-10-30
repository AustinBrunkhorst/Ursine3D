#include "Precompiled.h"

#include "EntityUtils.h"

#include <Editor.h>

JSFunction(CreateEntity)
{
    auto &world = GetCoreSystem( Editor )->GetProject( )->GetScene( ).GetWorld( );

    auto *entity = world.CreateEntity( );

    return CefV8Value::CreateUInt( entity->GetUniqueID( ) );
}