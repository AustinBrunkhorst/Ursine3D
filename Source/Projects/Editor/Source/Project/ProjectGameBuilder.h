#pragma once

#include "ProjectGameBuilderEvent.h"

#include <ResourcePipelineManager.h>

class ProjectGameBuilder : 
    public ursine::EventDispatcher<ProjectGameBuilderEventType>
{
public:
    ProjectGameBuilder(ursine::rp::ResourcePipelineManager &pipeline);

    void Build(
        const ursine::fs::path &launcherDir, 
        const ursine::fs::path &outputDir
    );

    void Cancel(void);

    bool IsBuilding(void) const;

private:
    bool m_building;

    ursine::rp::ResourcePipelineManager &m_pipeline;
};