/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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

#include "CoreSystem.h"

#include "KeyboardManagerEvent.h"

#include "KeyboardKey.h"
#include "KeyboardModifiers.h"

#include "UrsineTypes.h"

namespace ursine
{
    class KeyboardManager 
        : public core::CoreSystem
        , public EventDispatcher<KeyboardManagerEvent>
    {
        CORE_SYSTEM
    public:
        Meta(Enable)
        KeyboardManager(void);
        ~KeyboardManager(void);

        void OnInitialize(void) override;
        void OnRemove(void) override;

        Meta(Disable)
        static uint ScanToKey(SDL_Scancode scan);

        Meta(Disable)
        static uint KeyToScan(KeyboardKey key);

        bool IsDown(KeyboardKey key);
        bool IsUp(KeyboardKey key);

        bool IsTriggeredDown(KeyboardKey key);
        bool IsTriggeredUp(KeyboardKey key);

        KeyboardModifiers GetModifiers(void) const;

    private:
        uint8 m_stateQueue[SDL_NUM_SCANCODES];
        uint8 m_stateLast[SDL_NUM_SCANCODES];
        const uint8 *m_stateCurrent;

        KeyboardModifiers m_modifiers;

        void onAppUpdate(EVENT_HANDLER(Application));

        void onKeyDown(EVENT_HANDLER(Application));
        void onKeyUp(EVENT_HANDLER(Application));
        void onText(EVENT_HANDLER(Application));
    } Meta(Enable, WhiteListMethods);
}