#pragma once

#include <CoreSystem.h>

#include <Window.h>
#include <UIView.h>

#include "EditorTool.h"

#include <SDL.h>

class Editor : public ursine::core::CoreSystem
{
    CORE_SYSTEM
public:
    Editor(void);
    ~Editor(void);

    void OnInitialize(void) override;
    
private:
    ursine::Window *m_mainWindow;
    SDL_GLContext m_glContext;

    CefRefPtr<ursine::UIView> m_ui;

    std::vector<EditorTool *> m_tools;

    void initializeTools(void);

    void onAppUpdate(EVENT_HANDLER(ursine::Application));

    void onMainWindowResize(EVENT_HANDLER(ursine::Window));
} Meta(Enable);