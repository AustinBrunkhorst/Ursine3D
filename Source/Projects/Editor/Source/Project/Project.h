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
#include "ProjectGameBuilder.h"

#include "EditorGameContext.h"

#include "EditorSceneManager.h"
#include "EditorEntityManager.h"

#include "EditorResourcePipelineManager.h"

#include <ResourcePipelineManager.h>
#include <Scene.h>

class Project
{
public:
    Project(void);
    ~Project(void);

    ProjectConfig &GetConfig(void);
    void WriteConfig(void);

    ProjectGameBuilder &GetGameBuilder(void);

    ursine::rp::ResourcePipelineManager &GetResourcePipeline(void);
    ursine::resources::ResourceManager &GetBuiltInResourceManager(void);

    ursine::Scene &GetScene(void);

    void SetEmptyScene(void);

    const ursine::GUID &GetLastOpenedWorld(void);

    bool CreateEditorResource(const ursine::GUID &resourceGUID) const;

private:
    friend class Editor;

    Project(const Project &rhs) = delete;
    Project &operator=(const Project &rhs) = delete;

    void initialize(const ProjectConfig &config);

    void initializeScene(const ursine::resources::ResourceReference &startingWorld);

    void onSceneWorldChanged(EVENT_HANDLER(ursine::Scene));
    void onScenePlayStateChanged(EVENT_HANDLER(ursine::Scene));

    void onResourceModified(EVENT_HANDLER(ursine::rp::ResourcePipelineManager));

    ProjectConfig m_config;

    ursine::rp::ResourcePipelineManager m_resourcePipeline;
    ursine::resources::ResourceManager m_builtInResourceManager;

    ProjectGameBuilder m_gameBuilder;

    ursine::Scene m_scene;
    ursine::Json m_worldCache;

    EditorGameContext *m_gameContext;
    EditorSceneManager *m_sceneManager;
    EditorEntityManager *m_entityManager;
    EditorResourcePipelineManager *m_pipelineManager;

    ursine::GUID m_lastOpenedWorld;
};