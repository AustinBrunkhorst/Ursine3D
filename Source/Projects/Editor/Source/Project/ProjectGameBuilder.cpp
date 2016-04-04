#include "Precompiled.h"

#include "ProjectGameBuilder.h"

ProjectGameBuilder::ProjectGameBuilder(ursine::rp::ResourcePipelineManager &pipeline)
    : EventDispatcher( this )
    , m_building( false )
    , m_pipeline( pipeline ) { }

void ProjectGameBuilder::Build(
    const ursine::fs::path &launcherDir,
    const ursine::fs::path &outputDir
)
{

}

void ProjectGameBuilder::Cancel(void)
{
    
}

bool ProjectGameBuilder::IsBuilding(void) const
{
    return m_building;
}