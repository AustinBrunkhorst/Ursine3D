/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ProjectBuildSettings.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ProjectBuildSettingsComponent.h"

#include <Editor.h>
#include <Project.h>

using namespace ursine;

NATIVE_COMPONENT_DEFINITION( ProjectBuildSettings );

ProjectBuildSettings::ProjectBuildSettings(void)
    : BaseComponent( ) 
{
    auto *project = GetCoreSystem( Editor )->GetProject( );

    auto &config = project->GetConfig( );

    launchAfterBuild = config.launchAfterBuild;

    auto &settings = config.buildSettings;

    windowTitle = settings.windowTitle;
    windowFullScreen = settings.windowFullScreen;
    windowResizable = settings.windowResizable;

    try
    {
        startingWorld = GUIDStringGenerator( )( settings.startingWorld );
    }
    catch (...)
    {
        startingWorld = kNullGUID;
    }
}

ProjectBuildSettings::~ProjectBuildSettings(void) 
{
    
}

void ProjectBuildSettings::Save(void)
{
    auto *project = GetCoreSystem( Editor )->GetProject( );

    auto &config = project->GetConfig( );

    config.launchAfterBuild = launchAfterBuild;

    auto &settings = config.buildSettings;

    settings.windowTitle = windowTitle;
    settings.windowFullScreen = windowFullScreen;
    settings.windowResizable = windowResizable;
    settings.startingWorld = startingWorld.GetGUIDString( );

    project->WriteConfig( );

    NotificationConfig success;

    success.type = NOTIFY_INFO;
    success.header = "Action Complete";
    success.message = "Build settings saved.";
    success.dismissible = true;

    EditorPostNotification( success );
}

void ProjectBuildSettings::OnInitialize(void)
{
    
}