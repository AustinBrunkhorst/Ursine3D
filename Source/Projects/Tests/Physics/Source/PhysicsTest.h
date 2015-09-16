#pragma once

#include <CoreSystem.h>

#include <Window.h>
#include <UIView.h>

class PhysicsTest : public ursine::core::CoreSystem
{
    CORE_SYSTEM
public:
	PhysicsTest(void);
    ~PhysicsTest(void);

    Meta(Disable)
    void OnInitialize(void) override;

    Meta(Disable)
    void OnRemove(void) override;
    
private:
    ursine::Window *m_mainWindow;

    CefRefPtr<ursine::UIView> m_ui;

    void onAppUpdate(EVENT_HANDLER(ursine::Application));

    void onMainWindowResize(EVENT_HANDLER(ursine::Window));
} Meta(Enable);