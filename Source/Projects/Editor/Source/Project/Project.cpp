/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Project.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "Project.h"
#include "ResourcePipelineConfig.h"

namespace resource_pipeline = ursine::resources::pipeline;

namespace
{
    const auto kResourcesBuildDirectory = "Resources";
}

Project::Project(void)
{
    
}

Project::~Project(void)
{
    
}

const ProjectConfig &Project::GetConfig(void) const
{
    return m_config;
}

ursine::Scene &Project::GetScene(void)
{
    return m_scene;
}

void Project::initialize(const ProjectConfig &config)
{
    m_config = config;

    resource_pipeline::ResourcePipelineConfig resourceConfig;

    resourceConfig.buildDirectory = config.buildDirectory;

    resourceConfig.resourceDirectory = config.resourceDirectory;
    {
        // add the resource directory
        resourceConfig.resourceDirectory /= kResourcesBuildDirectory;
    }

    m_resourcePipeline.SetConfig( resourceConfig );
}