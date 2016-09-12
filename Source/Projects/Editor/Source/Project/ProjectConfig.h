/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ProjectConfig.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ProjectInstallSettings.h"

#include <FileSystem.h>
#include <GameSettings.h>

struct ProjectConfig
{
    std::string title;
    std::string resourceDirectory;
    std::string buildDirectory;

    bool launchAfterBuild;
    ursine::GameSettings buildSettings;
    ProjectInstallSettings installSettings;

    Meta(Disable)
    ursine::fs::path projectFile;

    Meta(Disable)
    ursine::fs::path preferencesDirectory;

    ProjectConfig(void) = default;

    static ProjectConfig CreateDefault(const std::string &title);
} Meta(Enable);