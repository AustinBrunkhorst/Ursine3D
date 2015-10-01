/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GamepadManager.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "GamepadManager.h"

#include <SDL.h>

namespace ursine
{
    CORE_SYSTEM_DEFINITION( GamepadManager );

    GamepadManager::GamepadManager(void)
        : EventDispatcher( this )
    {
        
    }

    GamepadManager::~GamepadManager(void)
    {
        
    }

    void GamepadManager::OnInitialize(void)
    {
        SDL_InitSubSystem(
            SDL_INIT_JOYSTICK |
            SDL_INIT_HAPTIC |
            SDL_INIT_GAMECONTROLLER
        );

        auto *app = Application::Instance;

        app->Connect( APP_UPDATE, this, &GamepadManager::onAppUpdate );

        app->GetPlatformEvents( ).Listener( this )
            .On( SDL_CONTROLLERDEVICEADDED, &GamepadManager::onGamepadAdded )
            .On( SDL_CONTROLLERDEVICEREMOVED, &GamepadManager::onGamepadRemoved )
            .On( SDL_CONTROLLERBUTTONDOWN, &GamepadManager::onButtonDown )
            .On( SDL_CONTROLLERBUTTONUP, &GamepadManager::onButtonUp );
    }

    void GamepadManager::OnRemove(void)
    {
        auto *app = Application::Instance;

        app->Disconnect( APP_UPDATE, this, &GamepadManager::onAppUpdate );

        app->GetPlatformEvents( ).Listener( this )
            .Off( SDL_CONTROLLERDEVICEADDED, &GamepadManager::onGamepadAdded )
            .Off( SDL_CONTROLLERDEVICEREMOVED, &GamepadManager::onGamepadRemoved )
            .Off( SDL_CONTROLLERBUTTONDOWN, &GamepadManager::onButtonDown )
            .Off( SDL_CONTROLLERBUTTONUP, &GamepadManager::onButtonUp );

        for (auto gamepad : m_gamepads)
            SDL_GameControllerClose( gamepad.controller );

        m_gamepads.clear( );

        m_virtualGamepads.clear( );
    }

    GamepadState *GamepadManager::GetState(ursine::GamepadIndex index)
    {
        auto gamepad = getGamepad( index );

        if (!gamepad)
            return nullptr;

        return &gamepad->state;
    }

    int GamepadManager::NumConnected(void) const
    {
        return SDL_NumJoysticks( );
    }

    void GamepadManager::Vibrate(GamepadIndex index, float intensity, TimeSpan duration)
    {
        auto gamepad = getGamepad( index );

        if (gamepad && gamepad->haptic)
            SDL_HapticRumblePlay( gamepad->haptic, intensity, duration.Milliseconds( ) );
    }

    void GamepadManager::StopAllVibration(void)
    {
        for (auto &gamepad : m_gamepads)
        {
            if (gamepad.haptic)
                SDL_HapticRumbleStop( gamepad.haptic );
        }
    }

    void GamepadManager::SetDeadZoneType(GamepadIndex index, GamepadDeadZoneType type)
    {
        auto gamepad = getGamepad( index );

        if (!gamepad)
            return;

        gamepad->dzType = type;
    }

    VirtualGamepad GamepadManager::CreateVirtualGamepad(void)
    {
        m_virtualGamepads.emplace_back( true );

        return { this, static_cast<GamepadIndex>( m_virtualGamepads.size( ) - 1u ) };
    }

    VirtualGamepad GamepadManager::GetVirtualGamepad(GamepadIndex index)
    {
        UAssert( IsVirtual( index ), 
            "Gamepad isn't virtual." );

        return { this, IndexFromVirtual( index ) };
    }

    bool GamepadManager::IsVirtual(GamepadIndex index)
    {
        return utils::IsFlagSet( index, VirtualGamepad::INDEX_FLAG );
    }

    GamepadIndex GamepadManager::IndexFromVirtual(GamepadIndex index)
    {
        return index & ~VirtualGamepad::INDEX_FLAG;
    }

    GamepadIndex GamepadManager::IndexToVirtual(GamepadIndex index)
    {
        return index | VirtualGamepad::INDEX_FLAG;
    }

    void GamepadManager::onAppUpdate(EVENT_HANDLER(Application))
    {
        // button args used for analog button event simulation
        GamepadButtonArgs analog_args = {
            GamepadButton( 0 ),
            GamepadIndex( 0 ),
            false,
            true
        };

        // non virtual gamepads
        for (uint i = 0; i < m_gamepads.size( ); ++i)
        {
            auto &gamepad = m_gamepads[ i ];

            gamepad.state.applyCurrentToLast( );

            // copy the actual current button states to the gamepad
            // BTN_INVALID is will always be one less than the first button index
            for (auto j = BTN_INVALID + 1; j < BTN_MAX; ++j)
            {
                gamepad.state.m_btnsCurrent[ j ] = SDL_GameControllerGetButton(
                    gamepad.controller,
                    static_cast<SDL_GameControllerButton>( j )
                );
            }

            gamepad.state.m_sticksCurrent.setLeft(
                SDL_GameControllerGetAxis( gamepad.controller, SDL_CONTROLLER_AXIS_LEFTX ),
                -SDL_GameControllerGetAxis( gamepad.controller, SDL_CONTROLLER_AXIS_LEFTY )
            );

            gamepad.state.m_sticksCurrent.setRight(
                SDL_GameControllerGetAxis( gamepad.controller, SDL_CONTROLLER_AXIS_RIGHTX ),
                -SDL_GameControllerGetAxis( gamepad.controller, SDL_CONTROLLER_AXIS_RIGHTY )
            );

            gamepad.state.m_sticksCurrent.applyDeadZone( gamepad.dzType, DEADZONE_SIZE );

            gamepad.state.m_triggersCurrent.setLeft(
                SDL_GameControllerGetAxis( gamepad.controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT )
            );

            gamepad.state.m_triggersCurrent.setRight(
                SDL_GameControllerGetAxis( gamepad.controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT )
            );

            analog_args.gamepad = i;

            // simulate button presses for analog buttons
            for (uint j = BTN_TRIGGER_LEFT; j < BTN_STICK_RIGHT_RIGHT; ++j)
            {
                if (gamepad.state.IsButtonTriggeredDown( static_cast<GamepadButton>( j ) ))
                {
                    analog_args.button = static_cast<GamepadButton>( j );
                    analog_args.pressed = true;

                    Dispatch( GP_BUTTON_DOWN, &analog_args );
                }
                else if (gamepad.state.IsButtonTriggeredUp( static_cast<GamepadButton>( j ) ))
                {
                    analog_args.button = static_cast<GamepadButton>( j );
                    analog_args.pressed = false;

                    Dispatch( GP_BUTTON_UP, &analog_args );
                }
            }
        }

        // virtual gamepads
        for (uint i = 0; i < m_virtualGamepads.size( ); ++i)
        {
            auto &gamepad = m_virtualGamepads[ i ];

            gamepad.state.applyCurrentToLast( );

            gamepad.state.m_sticksCurrent.applyDeadZone( gamepad.dzType, DEADZONE_SIZE );

            analog_args.gamepad = i;

            // simulate button presses for analog buttons
            for (uint j = BTN_TRIGGER_LEFT; j < BTN_STICK_RIGHT_RIGHT; ++j)
            {
                if (gamepad.state.IsButtonTriggeredDown( static_cast<GamepadButton>( j ) ))
                {
                    analog_args.button = static_cast<GamepadButton>( j );
                    analog_args.pressed = true;

                    Dispatch( GP_BUTTON_DOWN, &analog_args );
                }
                else if (gamepad.state.IsButtonTriggeredUp( static_cast<GamepadButton>( j ) ))
                {
                    analog_args.button = static_cast<GamepadButton>( j );
                    analog_args.pressed = false;

                    Dispatch( GP_BUTTON_UP, &analog_args );
                }
            }
        }
    }

    void GamepadManager::onGamepadAdded(EVENT_HANDLER(Application))
    {
        EVENT_ATTRS(Application, PlatformEventArgs);

        auto which = args->data->cdevice.which;

        // make sure this controller is mappable (SDL supports it)
        if (!SDL_IsGameController( which ))
            return;

        auto index = static_cast<GamepadIndex>( which );

        if (index + 1u > m_gamepads.size( ))
            m_gamepads.resize( index + 1u );

        auto &gamepad = m_gamepads[ index ];

        gamepad.controller = SDL_GameControllerOpen( which );

        UAssert(gamepad.controller, "Unable to open controller (%i).", which);

        gamepad.joystick = SDL_GameControllerGetJoystick( gamepad.controller );
        gamepad.instanceID = SDL_JoystickInstanceID( gamepad.joystick );

        gamepad.haptic = SDL_HapticOpenFromJoystick( gamepad.joystick );

        if (gamepad.haptic)
        {
            if (SDL_HapticRumbleInit( gamepad.haptic ) != 0)
                gamepad.haptic = nullptr;
        }

        gamepad.isValid = true;

        GamepadConnectionArgs e = { true, index };

        Dispatch( GP_CONNECTED, &e );
    }

    void GamepadManager::onGamepadRemoved(EVENT_HANDLER(Application))
    {
        EVENT_ATTRS(Application, PlatformEventArgs);

        auto instance_id = args->data->cdevice.which;

        GamepadIndex index;

        auto gamepad = getGamepad( instance_id, index );

        // couldn't find this gamepad
        if (!gamepad)
            return;

        GamepadConnectionArgs e = { false, index };

        // dispatch the disconnected event
        Dispatch( GP_DISCONNECTED, this, &e );

        if (gamepad->haptic)
            SDL_HapticClose( gamepad->haptic );

        // close the controller
        SDL_GameControllerClose( gamepad->controller );

        // set to nullptr instead of remove so we don't have to resize
        // again if the gamepad is re-connected
        gamepad->controller = nullptr;
        gamepad->joystick = nullptr;

        gamepad->isValid = false;
    }

    void GamepadManager::onButtonDown(EVENT_HANDLER(Application))
    {
        EVENT_ATTRS(Application, PlatformEventArgs);

        GamepadIndex index;

        auto gamepad = getGamepad( args->data->cbutton.which, index );

        // we aren't managing this gamepad
        if (!gamepad)
            return;

        auto button = static_cast<GamepadButton>( args->data->cbutton.button );

        GamepadButtonArgs e = {
            button,
            index,
            true,
            // we can assume this event is called before the gamepad manager
            // is updated
            gamepad->state.IsButtonUp( button )
        };

        Dispatch( GP_BUTTON_DOWN, &e );
    }

    void GamepadManager::onButtonUp(EVENT_HANDLER(Application))
    {
        EVENT_ATTRS(Application, PlatformEventArgs);

        GamepadIndex index;

        auto gamepad = getGamepad( args->data->cbutton.which, index );

        // we aren't managing this gamepad
        if (!gamepad)
            return;

        auto button = static_cast<GamepadButton>( args->data->cbutton.button );

        GamepadButtonArgs e = {
            button,
            index,
            false,
            gamepad->state.IsButtonDown( button )
        };

        Dispatch( GP_BUTTON_UP, &e );
    }

    GamepadManager::Gamepad *GamepadManager::getGamepad(int32 instance_id,
        GamepadIndex &index)
    {
        for (auto i = 0u; i < m_gamepads.size( ); ++i)
        {
            auto *gamepad = &m_gamepads[ i ];

            if (gamepad->instanceID == instance_id)
            {
                index = i;

                return gamepad;
            }
        }

        return nullptr;
    }

    GamepadManager::Gamepad *GamepadManager::getGamepad(GamepadIndex index)
    {
        // virtual
        if (IsVirtual( index ))
            return getVirtualGamepad( IndexFromVirtual( index ) );

        // invalid
        if (index >= m_gamepads.size( ))
            return nullptr;

        auto *gamepad = &m_gamepads[ index ];

        // disconnected
        if (!gamepad->isValid)
            return nullptr;

        return gamepad;
    }

    GamepadManager::Gamepad *GamepadManager::getVirtualGamepad(GamepadIndex index)
    {
        // doesn't exist
        if (index >= m_virtualGamepads.size( ))
            return nullptr;

        auto *gamepad = &m_virtualGamepads[ index ];

        // not valid
        if (!gamepad->isValid)
            return nullptr;

        return gamepad;
    }

    void GamepadManager::removeVirtualGamepad(GamepadIndex index)
    {
        m_virtualGamepads[ index ].isValid = false;
    }
}
