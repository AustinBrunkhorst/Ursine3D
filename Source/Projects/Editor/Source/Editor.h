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

#include "EditorPreferences.h"

#include "Project.h"
#include "EditorWindow.h"

#include <TimerManager.h>
#include <NotificationManager.h>

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

    const EditorPreferences &GetPreferences(void) const;
    Project *GetProject(void);

    ///////////////////////////////////////////////////////////////////////////
    // Notifications
    ///////////////////////////////////////////////////////////////////////////

    ursine::NotificationManager &GetNotificationManager(void);

    ursine::Notification PostNotification(const ursine::NotificationConfig &config);

    ///////////////////////////////////////////////////////////////////////////
    // Projects
    ///////////////////////////////////////////////////////////////////////////

    void CreateNewProject(const std::string &name, const std::string &directory);
    void LoadProject(const std::string &filename);

    void SetProjectStatus(const std::string &status);

private:
    struct
    {
        std::string uiEntryPoint;

        ursine::Vec2 windowSize;
        ursine::uint32 windowFlags;
        
        void (Editor::*updateHandler)(EVENT_HANDLER(ursine::Application));
    } m_startupConfig;
    
    ursine::graphics::GfxAPI *m_graphics;

    ursine::NotificationManager m_notificationManager;

    EditorWindow m_mainWindow;

    EditorPreferences m_preferences;
    Project *m_project;

    ursine::TimerID m_buildPipelineFocusTimeout;

    ///////////////////////////////////////////////////////////////////////////
    // Core System
    ///////////////////////////////////////////////////////////////////////////

    void OnInitialize(void) override;
    void OnRemove(void) override;

    ///////////////////////////////////////////////////////////////////////////
    // Preferences
    ///////////////////////////////////////////////////////////////////////////

    void loadPreferences(void);
    void writePreferences(void);

    std::string findAvailableProject(void) const;

    ///////////////////////////////////////////////////////////////////////////
    // Initialization
    ///////////////////////////////////////////////////////////////////////////

    void startup(void);

    void initializeWindow(void);
    void initializeGraphics(void);
    void initializeUI(void);

    void initializeProject(const std::string &filename);

    ///////////////////////////////////////////////////////////////////////////
    // Utilities
    ///////////////////////////////////////////////////////////////////////////

    void exitSplashScreen(void);

    ///////////////////////////////////////////////////////////////////////////
    // Event Handlers
    ///////////////////////////////////////////////////////////////////////////

    void onUILoaded(EVENT_HANDLER(ursine::UIView));

    void onLauncherUpdate(EVENT_HANDLER(ursine::Application));
    void onEditorUpdate(EVENT_HANDLER(ursine::Application));

    void onMainWindowResize(EVENT_HANDLER(ursine::Window));
    void onMainWindowFocusChanged(EVENT_HANDLER(ursine::Window));

    void onPipelinePreBuildItemStart(EVENT_HANDLER(ursine::rp::ResourcePipelineManager));
    void onPipelinePreBuildItemPreviewStart(EVENT_HANDLER(ursine::rp::ResourcePipelineManager));
    void onPipelinePreBuildComplete(EVENT_HANDLER(ursine::rp::ResourcePipelineManager));

    void onSceneWorldChanged(EVENT_HANDLER(ursine::Scene));
} Meta(Enable, WhiteListMethods);
