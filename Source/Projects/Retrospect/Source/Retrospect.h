#pragma once

#include <CoreSystem.h>

#include <Windows.h>
#include <ScreenManager.h>
#include <AudioManager.h>
#include <UIView.h>
#include <Scene.h>
#include <NativeJSFunction.h>

Meta(Enable, ExposeJavaScript)
JSFunction(InitGame);

Meta(Enable, ExposeJavaScript)
JSFunction(QuitGame);

class Retrospect : public ursine::core::CoreSystem
{
    CORE_SYSTEM
public:
    Meta(Enable)
    Retrospect(void);
    ~Retrospect(void);

    void OnInitialize(void) override;
    void OnRemove(void) override;

    ursine::ScreenManager *GetScreenManager(void) const;
    ursine::Window::Handle GetMainWindowHandle(void) const;
private:
    ursine::graphics::GfxAPI *m_graphics;

    ursine::ScreenManager *m_screenManager;

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