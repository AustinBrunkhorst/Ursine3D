/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WindowManager.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "CoreSystem.h"

#include "EventDispatcher.h"

#include "WindowManagerEvent.h"

#include "Window.h"

#include "Vec2.h"

#include <string>

namespace ursine
{
    class WindowManager
        : public core::CoreSystem
        , public EventDispatcher<WindowManagerEventType>
    {
        CORE_SYSTEM
    public:
        Meta(Enable)
        WindowManager(void);
        ~WindowManager(void);

        Window::Handle AddWindow(
            const std::string &title, 
            const Vec2 &location, 
            const Vec2 &size, 
            uint32 flags
        );

        // Creates a window from an existing window handle depending
        // on the platform (ex. HWND on Windows)
        Window::Handle CreateFrom(const void *data);

        Window::Handle GetWindow(uint32 windowID);

    private:
        friend class Window;

        std::unordered_map<uint32, Window::Handle> m_created;

        void destroy(Window *window);

        void onWindowEvent(EVENT_HANDLER(Application));
    } Meta(Enable, WhiteListMethods);
}