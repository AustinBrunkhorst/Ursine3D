/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Window.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "EventDispatcher.h"

#include "WindowConfig.h"

#include "WindowEvent.h"

#include "Vec2.h"

namespace ursine
{
    class WindowManager;

    class Window 
        : public EventDispatcher<WindowEventType>
    {
    public:
        typedef std::shared_ptr<Window> Handle;

        ~Window(void);

        WindowManager *GetManager(void);

        void SetTitle(const std::string &title);

        void SetBordered(bool bordered);
        void SetResizable(bool resizable);

        const Vec2 &GetSize(void) const;
        void SetSize(const Vec2 &size);

        const Vec2 &GetLocation(void) const;
        void SetLocation(const Vec2 &location);
        void SetLocationCentered(void);

        bool IsFocused(void) const;

        InternalWindowHandle GetInternalHandle(void) const;
        void *GetPlatformHandle(void);

        uint32 GetID(void) const;

        bool IsFullScreen(void) const;
        void SetFullScreen(bool fullscreen);

        bool IsMaximized(void) const;
        void SetMaximized(bool maximized);
        void SetMinimized(bool minimized);

        bool IsShown(void) const;
        void Show(bool show);

        float GetDPIScaleFactor(void) const;

        void SetIcon(const std::string &fileName);

        int GetDisplayIndex(void) const;

        std::vector<SDL_DisplayMode> GetAvailableDisplayModes(void) const;

        SDL_DisplayMode GetDisplayMode(void) const;
        void SetDisplayMode(const SDL_DisplayMode &mode);

        void Destroy(void);

    private:
        friend class WindowManager;

        Window(WindowManager *manager, InternalWindowHandle handle);

        // determines if the window is currently focused
        bool m_isFocused;

        // determines if the window is currently fullscreen
        bool m_isFullscreen;

        // determines if the window is currently shown
        bool m_isShown;

        // determines if this window is explicitly maximized
        bool m_isMaximized;

        uint32 m_id;

        float m_dpiScale;

        WindowManager *m_manager;
        InternalWindowHandle m_handle;

        Vec2 m_size;
        Vec2 m_location;

        void onWindowEvent(const SDL_WindowEvent &e);
    };
}