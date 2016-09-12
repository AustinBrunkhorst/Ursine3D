/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ProjectConfig.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ProjectConfig.h"

ProjectConfig ProjectConfig::CreateDefault(const std::string &title)
{
    ProjectConfig config;

    config.title = title;
    config.buildDirectory = "Build";
    config.resourceDirectory = "Resources";

    return config;
}