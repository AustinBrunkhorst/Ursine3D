/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ResourcePipelineConfig.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourcePipelineProperties.h"
#include "FileSystem.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            struct ResourcePipelineConfig
            {
                // Base directory to unmodified resources
                fs::path resourceDirectory;

                // Temporary directory where external processes can do things and things
                fs::path tempDirectory;

                // Directory where resources are built to
                fs::path buildDirectory;
            };
        }
    }

    namespace rp = resources::pipeline;
}