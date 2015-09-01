/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GamepadButton.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <SDL_gamecontroller.h>

namespace ursine
{
    enum GamepadButton
    {
        BTN_INVALID        = SDL_CONTROLLER_BUTTON_INVALID,
        BTN_A              = SDL_CONTROLLER_BUTTON_A,
        BTN_B              = SDL_CONTROLLER_BUTTON_B,
        BTN_X              = SDL_CONTROLLER_BUTTON_X,
        BTN_Y              = SDL_CONTROLLER_BUTTON_Y,
        BTN_DPAD_LEFT      = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
        BTN_DPAD_RIGHT     = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
        BTN_DPAD_UP        = SDL_CONTROLLER_BUTTON_DPAD_UP,
        BTN_DPAD_DOWN      = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
        BTN_STICK_LEFT     = SDL_CONTROLLER_BUTTON_LEFTSTICK,
        BTN_STICK_RIGHT    = SDL_CONTROLLER_BUTTON_RIGHTSTICK,
        BTN_SHOULDER_LEFT  = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
        BTN_SHOULDER_RIGHT = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
        BTN_BACK           = SDL_CONTROLLER_BUTTON_BACK,
        BTN_START          = SDL_CONTROLLER_BUTTON_START,
        BTN_GUIDE          = SDL_CONTROLLER_BUTTON_GUIDE,
        BTN_MAX            = SDL_CONTROLLER_BUTTON_MAX,
        BTN_TRIGGER_LEFT   = BTN_MAX + 1,
        BTN_TRIGGER_RIGHT,
        BTN_STICK_LEFT_UP,
        BTN_STICK_LEFT_DOWN,
        BTN_STICK_LEFT_LEFT,
        BTN_STICK_LEFT_RIGHT,
        BTN_STICK_RIGHT_UP,
        BTN_STICK_RIGHT_DOWN,
        BTN_STICK_RIGHT_LEFT,
        BTN_STICK_RIGHT_RIGHT,
        BTN_END
    };
}
