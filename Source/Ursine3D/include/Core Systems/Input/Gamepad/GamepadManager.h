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

#include "CoreSystem.h"
#include "EventDispatcher.h"

#include "GamepadManagerEvent.h"

#include "GamepadState.h"
#include "GamepadIndex.h"
#include "GamepadButton.h"
#include "GamepadTriggers.h"

#include "VirtualGamepad.h"

#include "TimeSpan.h"

#include <deque>

namespace ursine
{
    class GamepadManager 
        : public core::CoreSystem
        , public EventDispatcher<GamepadManagerEvent>
    {
        CORE_SYSTEM
    public:
        // Deadzone size for gamepads
        const float DEADZONE_SIZE = 0.18f;

        GamepadManager(void);
        ~GamepadManager(void);

        // Gets the current state of a gamepad based on the gamepad index.
        // returns null if the gamepad index is invalid or not connected
        GamepadState *GetState(ursine::GamepadIndex index);

        // Gets the number of connected gamepads currently
        int NumConnected(void) const;

        // Vibrates the given gamepad for the specified instensity [0-1] and duration
        void Vibrate(ursine::GamepadIndex index, float intensity, TimeSpan duration);

        // Stops vibration for all gamepads
        void StopAllVibration(void);

        // Sets how deadzones are handled for a specific gamepad
        void SetDeadZoneType(ursine::GamepadIndex index, GamepadDeadZoneType type);

        // Creates a new virtual gamepad
        VirtualGamepad CreateVirtualGamepad(void);

        // Gets an existing virtual gamepad
        VirtualGamepad GetVirtualGamepad(ursine::GamepadIndex index);

        // Determines if a gamepad is virtual
        static bool IsVirtual(ursine::GamepadIndex index);

        // Converts a virtual gamepad index to a non virtual index
        static GamepadIndex IndexFromVirtual(ursine::GamepadIndex index);

        // Converts a non virtual gamepad index to a virtual index
        static GamepadIndex IndexToVirtual(ursine::GamepadIndex index);

    private:
        friend class VirtualGamepad;

        // internal gamepad reference
        struct Gamepad
        {
            // if this isn't managed via SDL
            bool isVirtual;

            // if this gamepad is valid
            bool isValid;

            // instance id of this gamepad (not to be confused with device index)
            int instanceID;

            // deadzone type for this controller
            GamepadDeadZoneType dzType;

            // controller representing this gamepad (just a wrapper to 
            // SDL_Joystick which is a little frustrating)
            SDL_GameController *controller;

            // joystick representing this gamepad
            SDL_Joystick *joystick;

            // haptic feedback instance for this gamepad
            SDL_Haptic *haptic;

            // state of this gamepad
            GamepadState state;

            Gamepad(bool isVirtual = false)
                : isVirtual(isVirtual)
                , isValid(true)
                , instanceID(-1)
                , dzType(DZ_CIRCULAR)
                , controller(nullptr)
                , joystick(nullptr)
                , haptic(nullptr) { }
        };

        std::deque<Gamepad> m_gamepads;
        std::deque<Gamepad> m_virtualGamepads;

        void onAppUpdate(EVENT_HANDLER(Application));

        void onGamepadAdded(EVENT_HANDLER(Application));
        void onGamepadRemoved(EVENT_HANDLER(Application));

        void onButtonDown(EVENT_HANDLER(Application));
        void onButtonUp(EVENT_HANDLER(Application));

        // attempts to get a gamepad based on its instance id
        // outputs the gamepad index to index if found
        Gamepad *getGamepad(int32 instance_id, ursine::GamepadIndex &index);

        // attempts to get a gamepad based on the index
        Gamepad *getGamepad(ursine::GamepadIndex index);

        // attempts to get a virtual gamepad
        Gamepad *getVirtualGamepad(ursine::GamepadIndex index);

        // removes a virtual gamepad index
        void removeVirtualGamepad(ursine::GamepadIndex index);
    } Meta(Enable);
}
