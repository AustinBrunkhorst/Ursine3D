#pragma once

#include "EventDispatcher.h"

#include "WindowConfig.h"

#include "WindowEvent.h"

#include "Vec2.h"

namespace ursine
{
    class Window 
        : public EventDispatcher<WindowEventType>
    {
    public:
        ~Window(void);

        const Vec2 &GetSize(void) const;
        void SetSize(const Vec2 &size);

        const Vec2 &GetLocation(void) const;
        void SetLocation(const Vec2 &location);
        void SetLocationCentered(void);

        bool IsFocused(void) const;

        WindowHandle *GetHandle(void) const;
        void *GetPlatformHandle(void);

        uint32 GetID(void) const;

        bool IsFullScreen(void) const;
        void SetFullScreen(bool fullscreen);

        bool IsShown(void) const;
        void Show(bool show);

        int GetDisplayIndex(void) const;

        std::vector<SDL_DisplayMode> GetAvailableDisplayModes(void) const;

        SDL_DisplayMode GetDisplayMode(void) const;
        void SetDisplayMode(const SDL_DisplayMode &mode);

        void Destroy(void);

    private:
        friend class WindowManager;

        Window(WindowManager *manager, WindowHandle *handle);

        // determines if the window is currently focused
        bool m_isFocused;

        // determines if the window is currently fullscreen
        bool m_isFullscreen;

        // determines if the window is currently shown
        bool m_isShown;

        uint32 m_id;

        WindowManager *m_manager;
        WindowHandle *m_handle;

        Vec2 m_size;
        Vec2 m_location;

        void onWindowEvent(const SDL_WindowEvent &e);
    };
}