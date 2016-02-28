#include "Precompiled.h"

#include "EditorResourcePipelineManager.h"

#include "Editor.h"

using namespace ursine;

namespace
{
    namespace channel
    {
        const auto ResourcePipeline = "ResourcePipeline";
    }

    namespace events
    {
        namespace pipeline
        {
            const auto ResourceAdded = "ResourceAdded";
            const auto ResourceModified = "ResourceModified";
        }
    }
}

EditorResourcePipelineManager::EditorResourcePipelineManager(Project *project)
    : m_editor( GetCoreSystem( Editor ) )
    , m_pipeline( project->GetResourcePipeline( ) )
{
    m_pipeline.Listener( this )
        .On( rp::RP_RESOURCE_ADDED, &EditorResourcePipelineManager::onResourceAdded )
        .On( rp::RP_RESOURCE_MODIFIED, &EditorResourcePipelineManager::onResourceModified );
}

EditorResourcePipelineManager::~EditorResourcePipelineManager(void)
{
    m_pipeline.Listener( this )
        .Off( rp::RP_RESOURCE_ADDED, &EditorResourcePipelineManager::onResourceAdded )
        .Off( rp::RP_RESOURCE_MODIFIED, &EditorResourcePipelineManager::onResourceModified );
}

void EditorResourcePipelineManager::onResourceAdded(EVENT_HANDLER(rp::ResourcePipelineManager))
{
    EVENT_ATTRS(rp::ResourcePipelineManager, rp::ResourceChangeArgs);

    Json data = Json::object {
        { "guid", to_string( args->resource->GetGUID( ) ) }
    };

    m_editor->GetMainWindow( ).GetUI( )->Message(
        UI_CMD_BROADCAST,
        channel::ResourcePipeline,
        events::pipeline::ResourceAdded,
        data
    );
}

void EditorResourcePipelineManager::onResourceModified(EVENT_HANDLER(rp::ResourcePipelineManager))
{
    EVENT_ATTRS(rp::ResourcePipelineManager, rp::ResourceChangeArgs);

    Json data = Json::object {
        { "guid", to_string( args->resource->GetGUID( ) ) }
    };

    m_editor->GetMainWindow( ).GetUI( )->Message(
        UI_CMD_BROADCAST,
        channel::ResourcePipeline,
        events::pipeline::ResourceModified,
        data
    );
}