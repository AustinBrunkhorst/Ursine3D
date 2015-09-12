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
        WindowManager(void);
        ~WindowManager(void);

        Window *AddWindow(const std::string &title, const Vec2 &location, const Vec2 &size, uint32 flags);

    private:
        friend class Window;

        std::unordered_map<uint32, Window*> m_created;

        void destroy(Window *window);

        void onWindowEvent(EVENT_HANDLER(Application));
    } Meta(Enable);
}