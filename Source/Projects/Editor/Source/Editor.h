#pragma once

#include <CoreSystem.h>

#include <Window.h>
#include <UIView.h>

#include <NativeJSFunction.h>

class Project;
class NativeEditorTool;

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

    void InitializeScene(void);
    
private:
    ursine::graphics::GfxAPI *m_graphics;

    struct
    {
        ursine::Window *window;
        CefRefPtr<ursine::UIView> ui;
        GfxHND viewport;
        GfxHND camera;
    } m_mainWindow;

    Project *m_project;

    ursine::graphics::Model3D *m_skyBox;

    std::vector<NativeEditorTool *> m_tools;

    void initializeGraphics(void);

    void onAppUpdate(EVENT_HANDLER(ursine::Application));

    void onMainWindowResize(EVENT_HANDLER(ursine::Window));

    void onEntityAdded(EVENT_HANDLER(ursine::ecs::World));
} Meta(Enable);

Meta(Enable, ExposeJavaScript)
JSFunction(OnEditorUILoad);