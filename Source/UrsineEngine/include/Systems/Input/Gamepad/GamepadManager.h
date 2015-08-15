/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GamepadManager.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "System.h"

#include "GamepadManagerEvent.h"

#include "GamepadState.h"
#include "GamepadButton.h"
#include "GamepadTriggers.h"

#include "VirtualGamepad.h"

#include <vector>

namespace Ursine
{
    class GamepadManager 
        : public System
        , public EventDispatcher<GamepadManagerEvent>
    {
        friend class VirtualGamepad;

        // internal gamepad reference
        struct Gamepad
        {
            // if this isn't managed via SDL
            bool is_virtual;

            // if this gamepad is valid
            bool is_valid;

            // instance id of this gamepad (not to be confused with device index)
            int instance_id;

            // deadzone type for this controller
            GamepadDeadZoneType dz_type;

            // controller representing this gamepad (just a wrapper to 
            // SDL_Joystick which is a little frustrating)
            SDL_GameController *controller;

            // joystick representing this gamepad
            SDL_Joystick *joystick;

            // haptic feedback instance for this gamepad
            SDL_Haptic *haptic;

            // state of this gamepad
            GamepadState state;

            Gamepad(bool is_virtual = false)
                : is_virtual( is_virtual )
                , is_valid( true )
                , dz_type( DZ_CIRCULAR ) { }
        };

        std::deque<Gamepad> m_gamepads;
        std::deque<Gamepad> m_virtualGamepads;

        void onGamepadAdded(EVENT_HANDLER(Application));
        void onGamepadRemoved(EVENT_HANDLER(Application));

        void onButtonDown(EVENT_HANDLER(Application));
        void onButtonUp(EVENT_HANDLER(Application));

        // attempts to get a gamepad based on its instance id
        // outputs the gamepad index to index if found
        Gamepad *getGamepad(int32 instance_id, GamepadIndex &index);

        // attempts to get a gamepad based on the index
        Gamepad *getGamepad(GamepadIndex index);

        // attempts to get a virtual gamepad
        Gamepad *getVirtualGamepad(GamepadIndex index);

        // removes a virtual gamepad index
        void removeVirtualGamepad(GamepadIndex index);

    public:
        // Deadzone size for gamepads
        const float DEADZONE_SIZE = 0.18f;

        GamepadManager(void);
        ~GamepadManager(void);

        void Update(void) override;

        // Gets the current state of a gamepad based on the gamepad index.
        // returns null if the gamepad index is invalid or not connected
        GamepadState *GetState(GamepadIndex index);

        // Gets the number of connected gamepads currently
        int NumConnected(void) const;

        // Vibrates the given gamepad for the specified instensity [0-1] and duration
        void Vibrate(GamepadIndex index, float intensity, TimeSpan duration);

        // Stops vibration for all gamepads
        void StopAllVibration(void);

        // Sets how deadzones are handled for a specific gamepad
        void SetDeadZoneType(GamepadIndex index, GamepadDeadZoneType type);

        // Creates a new virtual gamepad
        VirtualGamepad CreateVirtualGamepad(void);

        // Gets an existing virtual gamepad
        VirtualGamepad GetVirtualGamepad(GamepadIndex index);

        // Determines if a gamepad is virtual
        static bool IsVirtual(GamepadIndex index);

        // Converts a virtual gamepad index to a non virtual index
        static GamepadIndex IndexFromVirtual(GamepadIndex index);

        // Converts a non virtual gamepad index to a virtual index
        static GamepadIndex IndexToVirtual(GamepadIndex index);
    };

    // global GamepadManager instance
    extern GamepadManager *gGamepadManager;
}
