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

                // Directory where resources are built to
                fs::path buildDirectory;
            };
        }
    }

    namespace rp = resources::pipeline;
}