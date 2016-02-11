/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** KeyboardManagerEvent.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "KeyboardKey.h"

namespace ursine
{
    enum KeyboardManagerEvent
    {
        KM_KEY_DOWN,
        KM_KEY_UP,
        KM_TEXT_INPUT
    };

    struct KeyboardKeyArgs : public EventArgs
    {
        // Determines if this key is down (true down, false up)
        bool state;

        // Key representing this event
        KeyboardKey key;

        // Number of times this event has repeated
        uint8 repeat;

        KeyboardKeyArgs(bool state, KeyboardKey key, uint8 repeat)
            : state(state)
            , key(key)
            , repeat(repeat) { }
    };

    struct KeyboardTextArgs : public EventArgs
    {
        std::string text;

        KeyboardTextArgs(const std::string text) 
            : text(text) { }
    };
}