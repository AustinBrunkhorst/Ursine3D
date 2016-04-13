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

JSFunction(GameContextIsMusicMuted)
{
    return CefV8Value::CreateBool(
        GetCoreSystem( GameLauncher )->GetScene( )->GetGameContext( )->IsMusicMuted( )
    );
}

JSFunction(GameContextMuteMusic)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid arguments.", nullptr );

    auto state = arguments[ 0 ]->GetBoolValue( );

    GetCoreSystem( GameLauncher )->GetScene( )->GetGameContext( )->MuteMusic( state );

    return CefV8Value::CreateBool( true );
}

JSFunction(GameContextIsAudioMuted)
{
    return CefV8Value::CreateBool(
        GetCoreSystem( GameLauncher )->GetScene( )->GetGameContext( )->IsAudioMuted( )
    );
}

JSFunction(GameContextMuteAudio)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid arguments.", nullptr );

    auto state = arguments[ 0 ]->GetBoolValue( );

    GetCoreSystem( GameLauncher )->GetScene( )->GetGameContext( )->MuteAudio( state );

    return CefV8Value::CreateBool( true );
}

JSFunction(GameContextExitGame)
{
    GetCoreSystem( GameLauncher )->GetScene( )->GetGameContext( )->ExitGame( );

    return CefV8Value::CreateBool( true );
}