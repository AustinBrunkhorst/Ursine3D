/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GameContextUtils.cpp
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

using namespace ursine;

JSFunction(GameContextGetWindowFullScreen)
{
    return CefV8Value::CreateBool(
        GetCoreSystem( Editor )->GetProject( )->GetScene( ).GetGameContext( )->GetWindowFullScreen( )
    );
}

JSFunction(GameContextSetWindowFullScreen)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid arguments.", nullptr );

    auto state = arguments[ 0 ]->GetBoolValue( );

    GetCoreSystem( Editor )->GetProject( )->GetScene( ).GetGameContext( )->SetWindowFullScreen( state );

    return CefV8Value::CreateBool( true );
}

JSFunction(GameContextGetVolume)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid arguments.", nullptr );

    auto outputType = arguments[ 0 ]->GetStringValue( );

    return CefV8Value::CreateDouble(
        GetCoreSystem( Editor )->GetProject( )->GetScene( ).GetGameContext( )->GetVolume( outputType )
    );
}

JSFunction(GameContextSetVolume)
{
    if (arguments.size( ) != 2)
        JSThrow( "Invalid arguments.", nullptr );

    auto volume = static_cast<float>( arguments[ 0 ]->GetDoubleValue( ) );
    auto outputType = arguments[ 1 ]->GetStringValue( );

    GetCoreSystem( Editor )->GetProject( )->GetScene( ).GetGameContext( )->SetVolume( volume, outputType );

    return CefV8Value::CreateBool( true );
}

JSFunction(GameContextExitGame)
{
    GetCoreSystem( Editor )->GetProject( )->GetScene( ).GetGameContext( )->ExitGame( );

    return CefV8Value::CreateBool( true );
}

JSFunction(GameContextGetAvailableManagers)
{
    Json::array managers;

    auto types = typeof( GameManager ).GetDerivedClasses( );

    for (auto &type : types)
        managers.emplace_back( type.GetName( ) );

    CefRefPtr<CefV8Value> items;

    JsonSerializer::Deserialize( managers, items );

    return items;
}
