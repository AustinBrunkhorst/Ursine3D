#include "Precompiled.h"

#include "SceneUtils.h"

#include <Editor.h>
#include <Project.h>

JSFunction(SceneGetActiveEntities)
{
    auto &scene = GetCoreSystem( Editor )->GetProject( )->GetScene( );

    auto &active = scene.GetWorld( ).GetActiveEntities( );

    auto ids = CefV8Value::CreateArray( active.size( ) );

    for (size_t i = 0; i < active.size( ); ++i)
    {
        ids->SetValue( 
            static_cast<int>( i ), 
            CefV8Value::CreateUInt( active[ i ]->GetUniqueID( ) )
        );
    }

    return ids;
}