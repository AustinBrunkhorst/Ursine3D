/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UISystem.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "CoreSystem.h"

#include "UISystem.h"
#include "UISystemEvent.h"

#include "Core.h"
#include "View.h"
#include "Space.h"

#include "Utils.h"

#include <cef_app.h>
#include <cef_runnable.h>

namespace ursine
{
    namespace ui
    {
        class UISystem
            : public core::CoreSystem
            , public EventDispatcher<UISystemEvent>
            , public CefApp
        {
            friend class Space;

            CefRefPtr<Core> m_core;
            CefRefPtr<View> m_view;

            void onGamepadButtonDown(EVENT_HANDLER(GamepadManager));
            void onGamepadButtonUp(EVENT_HANDLER(GamepadManager));
            void onGamepadConnected(EVENT_HANDLER(GamepadManager));
            void onGamepadDisconnected(EVENT_HANDLER(GamepadManager));

            void onKeyboardKeyDown(EVENT_HANDLER(KeyboardManager));
            void onKeyboardKeyUp(EVENT_HANDLER(KeyboardManager));
        public:
            static const int REMOTE_DEBUGGING_PORT = 6999;

            UISystem(const std::string &path);
            ~UISystem(void);

            CefRefPtr<View> GetView(void);

            URSINE_TODO("application event handler");
            void Update(void);

            void Message(const std::string &name, const Json &data);

            IMPLEMENT_REFCOUNTING(UISystem);
        };

        extern UISystem *gUISystem;
    }
}

DISABLE_RUNNABLE_METHOD_REFCOUNT(ursine::ui::UISystem);
