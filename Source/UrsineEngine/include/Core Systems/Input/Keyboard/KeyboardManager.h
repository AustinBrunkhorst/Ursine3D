/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** KeyboardManager.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "System.h"

#include "KeyboardManagerEvent.h"

#include "KeyboardKey.h"
#include "KeyboardModifiers.h"

#include "UrsineTypes.h"

namespace ursine
{
    class KeyboardManager 
        : public System
        , public EventDispatcher<KeyboardManagerEvent>
    {
        uint8 _state_queue[SDL_NUM_SCANCODES];
        uint8 _state_last[SDL_NUM_SCANCODES];
        const uint8 *_state_current;

        KeyboardModifiers _modifiers;

        void onKeyDown(EVENT_HANDLER(Application));
        void onKeyUp(EVENT_HANDLER(Application));
        void onText(EVENT_HANDLER(Application));

        void Update(void) override;

    public:
        KeyboardManager(void);
        ~KeyboardManager(void);

        static uint ScanToKey(SDL_Scancode scan);
        static uint KeyToScan(KeyboardKey key);

        bool IsDown(KeyboardKey key);
        bool IsUp(KeyboardKey key);

        bool IsTriggeredDown(KeyboardKey key);
        bool IsTriggeredUp(KeyboardKey key);

        KeyboardModifiers GetModifiers(void) const;
    };

    extern KeyboardManager *gKeyboardManager;
}