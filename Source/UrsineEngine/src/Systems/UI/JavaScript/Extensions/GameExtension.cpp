/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GameExtension.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/


#include "UrsinePrecompiled.h"

#include "GameExtension.h"
#include "UISystem.h"

#include "ScreenManager.h"

#include "MouseManager.h"
#include "GamepadManager.h"

namespace ursine
{
    namespace ui
    {
        GameExtension::GameExtension(void)
        {
            #define declare_bind(name) { STRINGIFY(name), BindJSMethod(&GameExtension::name) }

            static const std::pair<std::string, JavaScriptMethod> methods[]
            {
                declare_bind(ready),
                declare_bind(exit),

                declare_bind(showCursor),
                declare_bind(setCursorWarped),

                declare_bind(getAvailableDisplayModes),
                declare_bind(getDisplayMode),
                declare_bind(setDisplayMode),
                declare_bind(isFullScreen),
                declare_bind(setFullScreen),

                declare_bind(getTime),

                declare_bind(setScreen),
                declare_bind(addOverlay),
                declare_bind(removeScreen),
                declare_bind(removeCurrent),
                declare_bind(messageScreen),
                declare_bind(getFocusedScreen),

                declare_bind(getConnectedGamepads),
                declare_bind(gamepadVibrate)
            };

            #undef declare_bind

            for (auto &method : methods)
                bind( method.first, method.second );
        }

        GameExtension::~GameExtension(void) { }

        void GameExtension::Register(void)
        {
            const char * const source = JS_SRC(
                var Game = {
                    ready: function() {
                        native function ready();

                        return ready( );
                    },
                    exit: function() {
                        native function exit();

                        return exit( );
                    },
                    showCursor: function(show) {
                        native function showCursor();

                        return showCursor( show );
                    },
                    setCursorWarped: function(warped) {
                        native function setCursorWarped();

                        return setCursorWarped( warped );
                    },
                    getAvailableDisplayModes: function() {
                        native function getAvailableDisplayModes();

                        return getAvailableDisplayModes( );
                    },
                    getDisplayMode: function() {
                        native function getDisplayMode();

                        return getDisplayMode( );
                    },
                    setDisplayMode: function(mode) {
                        native function setDisplayMode();

                        return setDisplayMode( mode );
                    },
                    isFullScreen: function() {
                        native function isFullScreen();

                        return isFullScreen( );
                    },
                    setFullScreen: function(fullscreen) {
                        native function setFullScreen();

                        return setFullScreen( fullscreen );
                    },
                    getTime: function() {
                        native function getTime();

                        return getTime( );
                    },
                    Audio: {
                        isPlaying: function(name) {
                            native function audioIsPlaying();

                            return audioIsPlaying(name);
                        },
                        play: function(name, data) {
                            native function audioPlay();

                            if (data && !data.mode)
                                data.mode = 0;

                            return audioPlay(name, JSON.stringify(data || {}));
                        },
                        stop: function(name, fade) {
                            native function audioStop();

                            return audioStop(name, fade || true);
                        },
                        getVolume: function() {
                            native function audioGetVolume();

                            return audioGetVolume( );
                        },
                        setVolume: function(volume) {
                            native function audioSetVolume();

                            return audioSetVolume( volume );
                        }
                    },
                    ScreenManager: {
                        setScreen: function(name, data) {
                            native function setScreen();

                            return setScreen(name, JSON.stringify(data || {}));
                        },
                        addOverlay: function(name, data) {
                            native function addOverlay();

                            return addOverlay(name, JSON.stringify(data || {}));
                        },
                        removeScreen: function(id) {
                            native function removeScreen();

                            return removeScreen(id);
                        },
                        removeCurrent: function() {
                            native function removeCurrent();

                            return removeCurrent();
                        },
                        messageScreen: function(id, name, data) {
                            native function messageScreen();

                            return messageScreen(id, name, data);
                        },
                        getFocusedScreen: function() {
                            native function getFocusedScreen();

                            return getFocusedScreen();
                        }
                    },
                    GamepadManager: {
                        getConnected: function() {
                            native function getConnectedGamepads();

                            return getConnectedGamepads();
                        },
                        vibrate: function(index, intensity, duration) {
                            native function gamepadVibrate();

                            return gamepadVibrate( index, intensity, duration );
                        }
                    }
                };
            );

            CefRegisterExtension( "v8/game", source, this );
        }

        ////////////////////////////////////////////////////////////////////
        // General Methods
        ////////////////////////////////////////////////////////////////////

        JSMethod(GameExtension::ready)
        {
            {
                UAssert(gUISystem, "UISystem not created.");

                std::lock_guard<std::mutex> lock( _screen_manager_mutex );

                gUISystem->Dispatch( UI_GAME_READY, EventArgs::Empty );
            }

            return CefV8Value::CreateUndefined( );
        }

        JSMethod(GameExtension::exit)
        {
            UAssert(gApplication != nullptr, "No Application instance.");

            gApplication->Exit( );

            return CefV8Value::CreateUndefined( );
        }

        JSMethod(GameExtension::showCursor)
        {
            gMouseManager->ShowCursor( arguments[ 0 ]->GetBoolValue( ) );

            return CefV8Value::CreateUndefined( );
        }

        JSMethod(GameExtension::setCursorWarped)
        {
            gMouseManager->SetWarped( arguments[ 0 ]->GetBoolValue( ) );

            return CefV8Value::CreateUndefined( );
        }

        JSMethod(GameExtension::getAvailableDisplayModes)
        {
            auto modes = gWindowManager->GetAvailableDisplayModes( );

            auto jsModes = CefV8Value::CreateArray( modes.size( ) );

            for (uint32 i = 0; i < modes.size( ); ++i)
            {
                auto json = JsonSerializer::Serialize( modes[ i ] );

                auto value = CefV8Value::CreateObject( nullptr );

                JsonSerializer::Deserialize( json, value );

                jsModes->SetValue( i, value );
            }

            return jsModes;
        }

        JSMethod(GameExtension::getDisplayMode)
        {
            auto mode = gWindowManager->GetDisplayMode( );

            auto json = JsonSerializer::Serialize( mode );

            auto object = CefV8Value::CreateObject( nullptr );

            JsonSerializer::Deserialize( json, object );

            return object;
        }

        JSMethod(GameExtension::setDisplayMode)
        {
            auto data = JsonSerializer::Serialize( arguments[0] );

            SDL_DisplayMode mode;
            
            JsonSerializer::Deserialize( data, mode );

            // call on main thread
            Timer::Create( TimeSpan::Zero, kTimerGroupGlobal ).Completed([=]
            {
                std::lock_guard<std::mutex> lock( _timer_manager_mutex );

                gWindowManager->SetDisplayMode( mode );
            });

            return CefV8Value::CreateUndefined( );
        }

        JSMethod(GameExtension::isFullScreen)
        {
            return CefV8Value::CreateBool( gWindowManager->IsFullScreen( ) );
        }

        JSMethod(GameExtension::setFullScreen)
        {
            auto fullscreen = arguments[0]->GetBoolValue( );

            // call on main thread
            Timer::Create( TimeSpan::Zero, kTimerGroupGlobal ).Completed([=]
            {
                std::lock_guard<std::mutex> lock( _timer_manager_mutex );

                gWindowManager->SetFullScreen( fullscreen );
            });

            return CefV8Value::CreateUndefined( );
        }

        JSMethod(GameExtension::getTime)
        {
            return CefV8Value::CreateUInt( SDL_GetTicks( ) );
        }

        ////////////////////////////////////////////////////////////////////////
        // Screen Manager Methods
        ////////////////////////////////////////////////////////////////////////

        JSMethod(GameExtension::setScreen)
        {
            auto name = arguments[ 0 ]->GetStringValue( ).ToString( );

            std::string error;

            auto data =
                Json::parse( arguments[ 1 ]->GetStringValue( ).ToString( ), error );

            UAssert(error.empty(),
                "Error parsing screen data\n%s", error.c_str());

            {
                // this needs to be called on the main thread
                Timer::Create( TimeSpan::Zero, kTimerGroupGlobal ).Completed( [=]
                {
                    std::lock_guard<std::mutex> lock( _timer_manager_mutex );

                    gScreenManager->SetScreen( gScreenManager->Create( name ), data );
                });
            }

            return CefV8Value::CreateUndefined( );
        }

        JSMethod(GameExtension::addOverlay)
        {
            auto name = arguments[ 0 ]->GetStringValue( ).ToString( );

            std::string error;

            auto data =
                Json::parse( arguments[ 1 ]->GetStringValue( ).ToString( ), error );

            UAssert(error.empty(),
                "Error parsing screen data\n%s", error.c_str());

            // this needs to be called on the main thread
            Timer::Create( TimeSpan::Zero, kTimerGroupGlobal ).Completed( [=]
            {
                std::lock_guard<std::mutex> lock( _timer_manager_mutex );

                gScreenManager->AddOverlay( name, data );
            });

            return CefV8Value::CreateUndefined( );
        }

        JSMethod(GameExtension::removeScreen)
        {
            auto id = arguments[ 0 ]->GetIntValue( );

            {
                UAssert(gScreenManager, "ScreenManager not created.");

                std::lock_guard<std::mutex> lock( _screen_manager_mutex );

                gScreenManager->RemoveScreen( static_cast<ScreenID>( id ) );
            }

            return CefV8Value::CreateUndefined( );
        }

        JSMethod(GameExtension::removeCurrent)
        {
            {
                UAssert(gScreenManager, "ScreenManager not created.");

                std::lock_guard<std::mutex> lock( _screen_manager_mutex );

                gScreenManager->RemoveCurrent( );
            }

            return CefV8Value::CreateUndefined( );
        }

        JSMethod(GameExtension::messageScreen)
        {
            {
                UAssert(gScreenManager, "ScreenManager not created.");

                std::lock_guard<std::mutex> lock( _screen_manager_mutex );

                Json data = JsonSerializer::Serialize( arguments[ 2 ] );

                gScreenManager->MessageScreen(
                    // id
                    arguments[ 0 ]->GetIntValue( ),
                    // name
                    arguments[ 1 ]->GetStringValue( ),
                    // data
                    data );
            }

            return CefV8Value::CreateUndefined( );
        }

        JSMethod(GameExtension::getFocusedScreen)
        {
            UAssert(gScreenManager, "ScreenManager not created.");

            return CefV8Value::CreateUInt( gScreenManager->GetFocusedScreen( ) );
        }

        ////////////////////////////////////////////////////////////////////
        // Gamepad Manager Methods
        ////////////////////////////////////////////////////////////////////

        JSMethod(GameExtension::getConnectedGamepads)
        {
            UAssert(gGamepadManager, "GamepadManager not created.");

            CefRefPtr<CefV8Value> connected = CefV8Value::CreateArray( -1 );

            int last = 0;

            for (int i = 0; i < gGamepadManager->NumConnected( ); ++i)
            {
                if (gGamepadManager->GetState( i ) != nullptr)
                    connected->SetValue( last++, CefV8Value::CreateUInt( i ) );
            }

            return connected;
        }

        JSMethod(GameExtension::gamepadVibrate)
        {
            UAssert(gGamepadManager, "GamepadManager not created.");

            auto index = arguments[ 0 ]->GetUIntValue( );
            auto intensity = static_cast<float>( arguments[ 1 ]->GetDoubleValue( ) );
            auto duration = TimeSpan::FromMilliseconds( static_cast<float>( arguments[ 2 ]->GetUIntValue( ) ) );

            gGamepadManager->Vibrate( index, intensity, duration );

            return CefV8Value::CreateUndefined( );
        }
    }
}
