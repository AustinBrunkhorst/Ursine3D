/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Retrospect.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <CoreSystem.h>

#include <Windows.h>
#include <AudioManager.h>
#include <UIView.h>
#include <Scene.h>
#include <NativeJSFunction.h>

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

	ursine::AudioManager *m_audioManager;

    struct
    {
        ursine::Window::Handle window;
        ursine::UIView::Handle ui;
        ursine::graphics::Viewport *viewport;
    } m_mainWindow;

    void initializeGraphics(void);

    void onAppUpdate(EVENT_HANDLER(ursine::Application));

    void onMainWindowFocusChanged(EVENT_HANDLER(ursine::Window));
    void onMainWindowResize(EVENT_HANDLER(ursine::Window));
} Meta(Enable, WhiteListMethods);