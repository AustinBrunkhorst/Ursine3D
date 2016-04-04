/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GameLauncher.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <CoreSystem.h>

#include <UIView.h>
#include <Scene.h>
#include <GameSettings.h>

class GameLauncher : public ursine::core::CoreSystem
{
    CORE_SYSTEM;

public:
    Meta(Enable)
    GameLauncher(void);
    ~GameLauncher(void);

    void OnInitialize(void) override;
    void OnRemove(void) override;

    ursine::Window::Handle GetMainWindowHandle(void) const;

private:
    ursine::graphics::GfxAPI *m_graphics;

    ursine::Scene m_scene;
    ursine::GameSettings m_settings;

    struct
    {
        ursine::Window::Handle window;
        ursine::UIView::Handle ui;
        ursine::graphics::Viewport *viewport;
    } m_window;

    void initializeSettings(void);
    void initializeWindow(void);
    void initializeGraphics(void);
    void initalizeUI(void);

    void onAppUpdate(EVENT_HANDLER(ursine::Application));

    void onWindowFocusChanged(EVENT_HANDLER(ursine::Window));
    void onWindowResize(EVENT_HANDLER(ursine::Window));
} Meta(Enable, WhiteListMethods);