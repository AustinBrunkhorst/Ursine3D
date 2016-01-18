/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Editor.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <CoreSystem.h>

#include <Window.h>
#include <UIView.h>

#include "Project.h"

#include "Notifications/NotificationManager.h"

class NativeEditorTool;

class Editor : public ursine::core::CoreSystem
{
    CORE_SYSTEM
public:
    Meta(Enable, DisableNonDynamic)
    Editor(void);
    ~Editor(void);

    void OnInitialize(void) override;
    void OnRemove(void) override;
    
    ursine::Window::Handle GetMainWindow(void) const;
    ursine::UIView::Handle GetMainUI(void) const;

    Project::Handle GetProject(void) const;

    notification::NotificationManager &GetNotificationManager(void);

    notification::Notification PostNotification(const notification::NotificationConfig &config);
    
private:
    ursine::graphics::GfxAPI *m_graphics;

    notification::NotificationManager m_notificationManager;

    struct
    {
        ursine::Window::Handle window;
        ursine::UIView::Handle ui;
    } m_mainWindow;

    Project::Handle m_project;

    void initializeGraphics(void);
    void initializeScene(void);

    void onAppUpdate(EVENT_HANDLER(ursine::Application));

    void onFocusChange(EVENT_HANDLER( ursine::Window ));

    void onMainWindowResize(EVENT_HANDLER(ursine::Window));
} Meta(Enable, WhiteListMethods);
