/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** JSGameContext.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "JSGameContext.h"

#include "GameLauncher.h"

JSFunction(GameContextGetWindowFullScreen)
{
    return CefV8Value::CreateBool(
        GetCoreSystem( GameLauncher )->GetScene( )->GetGameContext( )->GetWindowFullScreen( )
    );
}

JSFunction(GameContextSetWindowFullScreen)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid arguments.", nullptr );

    auto state = arguments[ 0 ]->GetBoolValue( );

    ursine::Application::PostMainThread( [=] {
         GetCoreSystem( GameLauncher )->GetScene( )->GetGameContext( )->SetWindowFullScreen( state );
    } );

    return CefV8Value::CreateBool( true );
}

JSFunction(GameContextGetVolume)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid arguments.", nullptr );

    auto outputType = arguments[ 0 ]->GetStringValue( );

    return CefV8Value::CreateDouble(
        GetCoreSystem( GameLauncher )->GetScene( )->GetGameContext( )->GetVolume( outputType )
    );
}

JSFunction(GameContextSetVolume)
{
    if (arguments.size( ) != 2)
        JSThrow( "Invalid arguments.", nullptr );

    auto volume = static_cast<float>( arguments[ 0 ]->GetDoubleValue( ) );
    auto outputType = arguments[ 1 ]->GetStringValue( );

    ursine::Application::PostMainThread( [=] {
        GetCoreSystem( GameLauncher )->GetScene( )->GetGameContext( )->SetVolume( volume, outputType );
    } );

    return CefV8Value::CreateBool( true );
}

JSFunction(GameContextExitGame)
{
    GetCoreSystem( GameLauncher )->GetScene( )->GetGameContext( )->ExitGame( );

    return CefV8Value::CreateBool( true );
}