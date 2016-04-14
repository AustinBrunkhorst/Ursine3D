/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorResourcePipelineManager.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ProjectGameBuilder.h"
#include "ProjectGameInstaller.h"

#include <ResourcePipelineManager.h>

class Editor;
class Project;

class EditorResourcePipelineManager
{
public:
    EditorResourcePipelineManager(Project *project);
    ~EditorResourcePipelineManager(void);

private:
    Editor *m_editor;
    ProjectGameBuilder &m_gameBuilder;
    ProjectGameInstaller &m_gameInstaller;
    ursine::rp::ResourcePipelineManager &m_pipeline;

    ///////////////////////////////////////////////////////////////////////////
    // Project Build Events
    ///////////////////////////////////////////////////////////////////////////

    void onProjectBuildStart(EVENT_HANDLER(ProjectGameBuilder));
    void onProjectBuildProgress(EVENT_HANDLER(ProjectGameBuilder));
    void onProjectBuildComplete(EVENT_HANDLER(ProjectGameBuilder));   

    ///////////////////////////////////////////////////////////////////////////
    // Installer Build Events
    ///////////////////////////////////////////////////////////////////////////

    void onInstallStart(EVENT_HANDLER(ProjectGameInstaller));
    void onInstallComplete(EVENT_HANDLER(ProjectGameInstaller));

    ///////////////////////////////////////////////////////////////////////////
    // Resource Events
    ///////////////////////////////////////////////////////////////////////////

    void onResourceBuildStart(EVENT_HANDLER(ursine::rp::ResourcePipelineManager));
    void onResourceBuildComplete(EVENT_HANDLER(ursine::rp::ResourcePipelineManager));

    void onResourceAdded(EVENT_HANDLER(ursine::rp::ResourcePipelineManager));
    void onResourceModified(EVENT_HANDLER(ursine::rp::ResourcePipelineManager));
    void onResourceRemoved(EVENT_HANDLER(ursine::rp::ResourcePipelineManager));
    void onResourceRenamed(EVENT_HANDLER(ursine::rp::ResourcePipelineManager));
};