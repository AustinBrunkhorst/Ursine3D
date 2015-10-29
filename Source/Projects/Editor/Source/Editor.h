#pragma once

#include <CoreSystem.h>

#include <Window.h>
#include <UIView.h>

#include "Project.h"

class NativeEditorTool;

class Editor : public ursine::core::CoreSystem
{
    CORE_SYSTEM
public:
    Meta(Enable)
    Editor(void);
    ~Editor(void);

    void OnInitialize(void) override;
    void OnRemove(void) override;
    
    Project::Handle GetProject(void) const;
    
private:
    ursine::graphics::GfxAPI *m_graphics;

    struct
    {
        ursine::Window::Handle window;
        ursine::UIView::Handle ui;
    } m_mainWindow;

    Project::Handle m_project;

    void initializeGraphics(void);
    void initializeScene(void);

    void onAppUpdate(EVENT_HANDLER(ursine::Application));

    void onMainWindowResize(EVENT_HANDLER(ursine::Window));
} Meta(Enable, WhiteListMethods);