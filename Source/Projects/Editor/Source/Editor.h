#pragma once

#include <CoreSystem.h>

#include <Window.h>
#include <UIView.h>

#include "Project.h"
#include "EditorTool.h"

#include <SDL_video.h>

class Editor : public ursine::core::CoreSystem
{
    CORE_SYSTEM
public:
    Editor(void);
    ~Editor(void);

    Meta(Disable)
    void OnInitialize(void) override;

    Meta(Disable)
    void OnRemove(void) override;
    
private:
    ursine::Window *m_mainWindow;
    SDL_GLContext m_glContext;

    CefRefPtr<ursine::UIView> m_ui;

    Project *m_project;

    std::vector<EditorTool *> m_tools;

    void initializeTools(void);

    void onAppUpdate(EVENT_HANDLER(ursine::Application));

    void onMainWindowResize(EVENT_HANDLER(ursine::Window));
} Meta(Enable);