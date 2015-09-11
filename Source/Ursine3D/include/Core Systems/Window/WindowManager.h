#pragma once

#include "CoreSystem.h"

#include "WindowManagerEvent.h"
#include "WindowManagerConfig.h"

#include "Color.h"

#include <SDL_video.h>

namespace ursine
{
    class WindowManager
        : public core::CoreSystem
        , public EventDispatcher<WindowManagerEventType>
    {
        // determines if the window is currently focused
        bool m_isFocused;

        // determines if the window is currently fullscreen
        bool m_isFullscreen;

        // determines if the window is currently shown
        bool m_isShown;

        WindowHandle *m_handle;

        // OpenGL context for the current window
        SDL_GLContext m_glContext;

        Vec2 m_size;
        Vec2 m_location;

        Color m_bgColor;

        void onWindowEvent(EVENT_HANDLER(Application));

    public:
        WindowManager(const char *title, int width, int height,
            uint32 window_flags,
            int x = SDL_WINDOWPOS_CENTERED,
            int y = SDL_WINDOWPOS_CENTERED);

        ~WindowManager(void);

        const Vec2 &GetSize(void) const;
        void SetSize(const Vec2 &size);

        const Vec2 &GetLocation(void) const;
        void SetLocation(const Vec2 &location);
        void SetLocationCentered(void);

        bool IsFocused(void) const;

        WindowHandle *GetHandle(void);
        void *GetPlatformHandle(void);

        bool IsFullScreen(void) const;
        void SetFullScreen(bool fullscreen);

        bool IsShown(void) const;
        void Show(bool show);

        int GetDisplayIndex(void) const;

        std::vector<SDL_DisplayMode> GetAvailableDisplayModes(void) const;

        SDL_DisplayMode GetDisplayMode(void) const;
        void SetDisplayMode(const SDL_DisplayMode &mode);
    };

    extern template
    Json JsonSerializer::Serialize(SDL_DisplayMode &instance);

    extern template
    void JsonSerializer::Deserialize(const Json &data, SDL_DisplayMode &out);
}