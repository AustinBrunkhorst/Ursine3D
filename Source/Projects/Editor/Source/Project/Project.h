/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Project.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ProjectConfig.h"

#include <ResourcePipelineManager.h>
#include <Scene.h>

class Project
{
public:
    Project(void);
    ~Project(void);

    const ProjectConfig &GetConfig(void) const;

    ursine::resources::pipeline::ResourcePipelineManager &GetResourcePipeline(void);
    ursine::Scene &GetScene(void);

private:
    friend class Editor;

    Project(const Project &rhs) = delete;
    Project &operator=(const Project &rhs) = delete;

    void initialize(const ProjectConfig &config);

    ProjectConfig m_config;

    ursine::resources::pipeline::ResourcePipelineManager m_resourcePipeline;

    ursine::Scene m_scene;
};