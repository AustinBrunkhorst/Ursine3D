#pragma once

#include <CoreSystem.h>

#include <Windows.h>
#include <UIView.h>
#include <Scene.h>
#include <NativeJSFunction.h>

Meta(Enable, ExposeJavaScript)
JSFunction(GameInit);

class Retrospect : public ursine::core::CoreSystem
{
    CORE_SYSTEM
public:
    Meta(Enable)
    Retrospect(void);
    ~Retrospect(void);

    void OnInitialize(void) override;
    void OnRemove(void) override;

private:
    ursine::graphics::GfxAPI *m_graphics;

    struct
    {
        ursine::Window::Handle window;
        ursine::UIView::Handle ui;
    } m_mainWindow;

    ursine::Scene::Handle m_scene;

    void initializeGraphics(void);
    void initializeScene(void);

    void onAppUpdate(EVENT_HANDLER(ursine::Application));

    void onMainWindowResize(EVENT_HANDLER(ursine::Window));
} Meta(Enable, WhiteListMethods);