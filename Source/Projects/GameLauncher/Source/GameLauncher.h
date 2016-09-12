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

#include "GameLauncherGameContext.h"

#include <UIView.h>
#include <Scene.h>
#include <GameSettings.h>

class GameLauncher : public ursine::core::CoreSystem
{
    CORE_SYSTEM;

public:
    Meta(Enable)
    GameLauncher(void);
    GameLauncher(const GameLauncher &rhs);
    ~GameLauncher(void);

    ursine::Scene *GetScene(void);
    ursine::Window::Handle GetWindow(void);
    ursine::UIView::Handle GetUI(void);

private:
    ursine::graphics::GfxAPI *m_graphics;

    ursine::Scene *m_scene;
    ursine::GameSettings m_settings;

    GameLauncherGameContext *m_gameContext;

    struct
    {
        ursine::Window::Handle window;
        ursine::UIView::Handle ui;
        ursine::graphics::Viewport *viewport;
    } m_window;

    GameLauncher &operator=(const GameLauncher &rhs) = delete;

    void OnInitialize(void) override;
    void OnRemove(void) override;

    void initSettings(void);
    void initWindow(void);
    void initGraphics(void);
    void initUI(void);
    void initBuildSettings(void);

    void onAppUpdate(EVENT_HANDLER(ursine::Application));

    void onWindowFocusChanged(EVENT_HANDLER(ursine::Window));
    void onWindowResize(EVENT_HANDLER(ursine::Window));
} Meta(Enable, WhiteListMethods);