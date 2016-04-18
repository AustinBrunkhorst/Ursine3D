/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ProjectInstallSettings.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

struct ProjectInstallSettings
{
    std::string gameName;
    std::string publisherName;
    std::string displayName;
    std::string brandingText;
    unsigned versionMajor;
    unsigned versionMinor;
    unsigned versionBuild;
    std::string versionDisplayName;
    std::string defaultStartMenuLocation;

    ProjectInstallSettings(void) = default;
} Meta(Enable);