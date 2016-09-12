/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ProjectGameInstallerEvent.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

enum ProjectGameInstallerEventType
{
    PIB_BUILD_START,
    PIB_BUILD_COMPLETE
};

struct ProjectGameInstallerBuildCompleteArgs : ursine::EventArgs
{
    bool successful;
    std::string error;
};