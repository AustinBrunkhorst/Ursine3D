#pragma once

#include <CoreSystem.h>

#include <Window.h>
#include <UIView.h>

#include "EditorTool.h"

#include <SDL_video.h>

class Project;

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
    
    Project *GetProject(void) const;
    
private:
    ursine::GfxAPI *m_graphics;

    struct
    {
        ursine::Window *window;
        CefRefPtr<ursine::UIView> ui;
        GFXHND viewport;
        GFXHND camera;
    } m_mainWindow;

    Project *m_project;

    std::vector<EditorTool *> m_tools;

    void initializeGraphics(void);
    void initializeTools(void);

    void resizeMainWindow(int width, int height);

    void onAppUpdate(EVENT_HANDLER(ursine::Application));

    void onMainWindowResize(EVENT_HANDLER(ursine::Window));
} Meta(Enable);