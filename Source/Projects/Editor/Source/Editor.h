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

#include <NotificationManager.h>

#include "Project.h"
#include "EditorWindow.h"

class NativeEditorTool;

class Editor : public ursine::core::CoreSystem
{
    CORE_SYSTEM
public:
    Meta(Enable, DisableNonDynamic)
    Editor(void);
    ~Editor(void);

    const EditorWindow &GetMainWindow(void) const;

    Project &GetProject(void);

    ursine::NotificationManager &GetNotificationManager(void);
    ursine::Notification PostNotification(const ursine::NotificationConfig &config);
    
private:
    void OnInitialize(void) override;
    void OnRemove(void) override;

    ursine::graphics::GfxAPI *m_graphics;

    ursine::NotificationManager m_notificationManager;

    EditorWindow m_mainWindow;

    Project m_project;

    void initializeGraphics(void);
    void initializeUI(void);

    void onAppUpdate(EVENT_HANDLER(ursine::Application));
    void onMainWindowResize(EVENT_HANDLER(ursine::Window));
} Meta(Enable, WhiteListMethods);
