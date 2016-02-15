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

class Editor : public ursine::core::CoreSystem
{
    CORE_SYSTEM;

public:
    Meta(Enable, DisableNonDynamic)
    Editor(void);
    ~Editor(void);

    ///////////////////////////////////////////////////////////////////////////
    // Misc
    ///////////////////////////////////////////////////////////////////////////

    const EditorWindow &GetMainWindow(void) const;

    Project &GetProject(void);

    ///////////////////////////////////////////////////////////////////////////
    // Notifications
    ///////////////////////////////////////////////////////////////////////////

    ursine::NotificationManager &GetNotificationManager(void);

    ursine::Notification PostNotification(const ursine::NotificationConfig &config);

private:
    struct
    {
        std::string uiEntryPoint;

        ursine::Vec2 windowSize;
        
        void (Editor::*updateHandler)(EVENT_HANDLER(ursine::Application));
    } m_startupConfig;
    
    ursine::graphics::GfxAPI *m_graphics;

    ursine::NotificationManager m_notificationManager;

    EditorWindow m_mainWindow;

    Project m_project;

    ///////////////////////////////////////////////////////////////////////////
    // Core System
    ///////////////////////////////////////////////////////////////////////////

    void OnInitialize(void) override;
    void OnRemove(void) override;

    ///////////////////////////////////////////////////////////////////////////
    // Initialization
    ///////////////////////////////////////////////////////////////////////////

    void startup(void);

    void initializeWindow(void);
    void initializeGraphics(void);
    void initializeUI(void);

    ///////////////////////////////////////////////////////////////////////////
    // Event Handlers
    ///////////////////////////////////////////////////////////////////////////

    void onLauncherUpdate(EVENT_HANDLER(ursine::Application));
    void onEditorUpdate(EVENT_HANDLER(ursine::Application));

    void onMainWindowResize(EVENT_HANDLER(ursine::Window));
} Meta(Enable, WhiteListMethods);
