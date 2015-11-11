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
    Meta(Enable)
    Editor(void);
    ~Editor(void);

    void OnInitialize(void) override;
    void OnRemove(void) override;
    
    Project *GetProject(void) const;
    
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

    void initializeGraphics(void);
    void initializeScene(void);

    void onAppUpdate(EVENT_HANDLER(ursine::Application));

    void onMainWindowResize(EVENT_HANDLER(ursine::Window));

    void onEntityAdded(EVENT_HANDLER(ursine::ecs::World));
    void onComponentChanged(EVENT_HANDLER(ursine::ecs::World));
} Meta(Enable, WhiteListMethods);