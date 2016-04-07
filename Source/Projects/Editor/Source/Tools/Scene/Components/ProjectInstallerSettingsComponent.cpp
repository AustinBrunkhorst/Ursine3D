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

#include "ProjectInstallerSettingsComponent.h"

#include <Editor.h>
#include <Project.h>

using namespace ursine;

NATIVE_COMPONENT_DEFINITION( ProjectInstallerSettings );

ProjectInstallerSettings::ProjectInstallerSettings(void)
    : BaseComponent( ) 
{
    auto *project = GetCoreSystem( Editor )->GetProject( );

    auto &config = project->GetConfig( );
    auto &settings = config.installSettings;

    gameName = settings.gameName;
    publisherName = settings.publisherName;
    displayName = settings.displayName;
    brandingText = settings.brandingText;
    versionMajor = settings.versionMajor;
    versionMinor = settings.versionMinor;
    versionBuild = settings.versionBuild;
    versionDisplayName = settings.versionDisplayName;
    defaultStartMenuLocation = settings.defaultStartMenuLocation;
}

ProjectInstallerSettings::~ProjectInstallerSettings(void)
{
    
}

void ProjectInstallerSettings::Save(void)
{
    auto *project = GetCoreSystem( Editor )->GetProject( );

    auto &config = project->GetConfig( );
    auto &settings = config.installSettings;

    settings.gameName = gameName;
    settings.publisherName = publisherName;
    settings.displayName = displayName;
    settings.brandingText = brandingText;
    settings.versionMajor = versionMajor;
    settings.versionMinor = versionMinor;
    settings.versionBuild = versionBuild;
    settings.versionDisplayName = versionDisplayName;
    settings.defaultStartMenuLocation = defaultStartMenuLocation;

    project->WriteConfig( );

    NotificationConfig success;

    success.type = NOTIFY_INFO;
    success.header = "Action Complete";
    success.message = "Installer settings saved.";
    success.dismissible = true;

    EditorPostNotification( success );
}

void ProjectInstallerSettings::OnInitialize(void)
{
    
}