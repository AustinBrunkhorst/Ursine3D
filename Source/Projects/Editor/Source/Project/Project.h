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
#include "ProjectGameInstaller.h"

#include "ProjectPreferenceStore.h"

#include "EditorGameContext.h"

#include "EditorScene.h"

#include "EditorSceneManager.h"
#include "EditorEntityManager.h"

#include "EditorResourcePipelineManager.h"

#include <ResourcePipelineManager.h>

class Project
{
public:
    Project(void);
    ~Project(void);

    ProjectConfig &GetConfig(void);
    void WriteConfig(void);

    ProjectPreferenceStore &GetPreferenceStore(void);

    ProjectGameBuilder &GetGameBuilder(void);
    ProjectGameInstaller &GetGameInstaller(void);

    ursine::rp::ResourcePipelineManager &GetResourcePipeline(void);
    ursine::resources::ResourceManager &GetBuiltInResourceManager(void);

    EditorScene &GetScene(void);

    void SetEmptyScene(void);

    const ursine::GUID &GetLastOpenedWorld(void);

    bool CreateEditorResource(const ursine::GUID &resourceGUID) const;

    void StartGame(void);
    void StartPlaying(void);

private:
    friend class Editor;

    Project(const Project &rhs) = delete;
    Project &operator=(const Project &rhs) = delete;

    void initialize(const ProjectConfig &config);

    void initializeScene(const ursine::resources::ResourceReference &startingWorld);

    bool inGameRunMode(void) const;

    void onSceneWorldChanged(EVENT_HANDLER(ursine::Scene));
    void onScenePlayStateChanged(EVENT_HANDLER(ursine::Scene));

    void onResourceModified(EVENT_HANDLER(ursine::rp::ResourcePipelineManager));


    ProjectConfig m_config;

    ProjectPreferenceStore m_preferences;

    ursine::rp::ResourcePipelineManager m_resourcePipeline;
    ursine::resources::ResourceManager m_builtInResourceManager;

    ProjectGameBuilder m_gameBuilder;
    ProjectGameInstaller m_gameInstaller;

    EditorScene m_scene;
    ursine::Json m_worldCache;
    ursine::resources::ResourceReference m_playedWorld;

    EditorGameContext *m_gameContext;
    EditorSceneManager *m_sceneManager;
    EditorEntityManager *m_entityManager;
    EditorResourcePipelineManager *m_pipelineManager;

    ursine::GUID m_lastOpenedWorld;
};