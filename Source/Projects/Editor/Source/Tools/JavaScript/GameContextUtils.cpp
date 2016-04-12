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

JSFunction(GameContextIsMusicMuted)
{
    return CefV8Value::CreateBool(
        GetCoreSystem( Editor )->GetProject( )->GetScene( ).GetGameContext( )->IsMusicMuted( )
    );
}

JSFunction(GameContextMuteMusic)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid arguments.", nullptr );

    auto state = arguments[ 0 ]->GetBoolValue( );

    GetCoreSystem( Editor )->GetProject( )->GetScene( ).GetGameContext( )->MuteMusic( state );

    return CefV8Value::CreateBool( true );
}

JSFunction(GameContextIsAudioMuted)
{
    return CefV8Value::CreateBool(
        GetCoreSystem( Editor )->GetProject( )->GetScene( ).GetGameContext( )->IsAudioMuted( )
    );
}

JSFunction(GameContextMuteAudio)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid arguments.", nullptr );

    auto state = arguments[ 0 ]->GetBoolValue( );

    GetCoreSystem( Editor )->GetProject( )->GetScene( ).GetGameContext( )->MuteAudio( state );

    return CefV8Value::CreateBool( true );
}

JSFunction(GameContextExitGame)
{
    GetCoreSystem( Editor )->GetProject( )->GetScene( ).GetGameContext( )->ExitGame( );

    return CefV8Value::CreateBool( true );
}
