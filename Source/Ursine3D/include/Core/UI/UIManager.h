/* ---------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UIManager.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "CoreSystem.h"

#include "UIManager.h"
#include "UIManagerEvent.h"

#include "Core.h"
#include "UIView.h"

#include "Window.h"

#include <cef_app.h>
#include <cef_runnable.h>

namespace ursine
{
    class UIManager
        : public core::CoreSystem
        , public EventDispatcher<UIManagerEvent>
        , public CefApp
    {
        CORE_SYSTEM;
    public:
        static const int REMOTE_DEBUGGING_PORT = 6999;

        UIManager(void);
        ~UIManager(void);

        CefRefPtr<UIView> CreateView(Window *window, const std::string &url);

    private:
        CefRefPtr<Core> m_core;

        void onAppUpdate(EVENT_HANDLER(Application));

        IMPLEMENT_REFCOUNTING(UIManager);
    } Meta(Enable);
}

DISABLE_RUNNABLE_METHOD_REFCOUNT(ursine::UIManager);
