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

#include "EditorEntityManager.h"

class Project
{
public:
    Project(void);
    ~Project(void);

    const ProjectConfig &GetConfig(void) const;

    ursine::resources::pipeline::ResourcePipelineManager &GetResourcePipeline(void);
    ursine::Scene &GetScene(void);

    void SetEmptyScene(void);

    const ursine::GUID &GetLastOpenedWorld(void);

private:
    friend class Editor;

    Project(const Project &rhs) = delete;
    Project &operator=(const Project &rhs) = delete;

    void initialize(const ProjectConfig &config);

    void initializeScene(const ursine::resources::ResourceReference &startingWorld);

    void onSceneWorldChanged(EVENT_HANDLER(ursine::Scene));

    void onResourceAdded(EVENT_HANDLER(ursine::rp::ResourcePipelineManager));
    void onResourceModified(EVENT_HANDLER(ursine::rp::ResourcePipelineManager));

    ProjectConfig m_config;

    ursine::rp::ResourcePipelineManager m_resourcePipeline;

    ursine::Scene m_scene;

    EditorEntityManager *m_entityManager;

    ursine::GUID m_lastOpenedWorld;
};