#include "Precompiled.h"

#include "EditorResourcePipelineManager.h"

#include "Editor.h"

#include <ResourceItem.h>

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
            const auto ResourceRemoved = "ResourceRemoved";
            const auto ResourceRenamed = "ResourceRenamed";
        }
    }
}

EditorResourcePipelineManager::EditorResourcePipelineManager(Project *project)
    : m_editor( GetCoreSystem( Editor ) )
    , m_pipeline( project->GetResourcePipeline( ) )
{
    m_pipeline.Listener( this )
        .On( rp::RP_RESOURCE_ADDED, &EditorResourcePipelineManager::onResourceAdded )
        .On( rp::RP_RESOURCE_MODIFIED, &EditorResourcePipelineManager::onResourceModified )
        .On( rp::RP_RESOURCE_REMOVED, &EditorResourcePipelineManager::onResourceRemoved )
        .On( rp::RP_RESOURCE_RENAMED, &EditorResourcePipelineManager::onResourceRenamed );
}

EditorResourcePipelineManager::~EditorResourcePipelineManager(void)
{
    m_pipeline.Listener( this )
        .Off( rp::RP_RESOURCE_ADDED, &EditorResourcePipelineManager::onResourceAdded )
        .Off( rp::RP_RESOURCE_MODIFIED, &EditorResourcePipelineManager::onResourceModified )
        .Off( rp::RP_RESOURCE_REMOVED, &EditorResourcePipelineManager::onResourceRemoved )
        .Off( rp::RP_RESOURCE_RENAMED, &EditorResourcePipelineManager::onResourceRenamed );
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

void EditorResourcePipelineManager::onResourceRemoved(EVENT_HANDLER(rp::ResourcePipelineManager))
{
    EVENT_ATTRS(rp::ResourcePipelineManager, rp::ResourceChangeArgs);

    // the resource is removed, so we have to bake the info
    Json data = Json::object {
        { "resource", JsonSerializer::Serialize( args->resource ) }
    };

    m_editor->GetMainWindow( ).GetUI( )->Message(
        UI_CMD_BROADCAST,
        channel::ResourcePipeline,
        events::pipeline::ResourceRemoved,
        data
    );
}

void EditorResourcePipelineManager::onResourceRenamed(EVENT_HANDLER(rp::ResourcePipelineManager))
{
    EVENT_ATTRS(rp::ResourcePipelineManager, rp::ResourceRenameArgs);

    Json data = Json::object {
        { "guid", to_string( args->resource->GetGUID( ) ) },
        { "oldName", args->oldName.string( ) }
    };

    m_editor->GetMainWindow( ).GetUI( )->Message(
        UI_CMD_BROADCAST,
        channel::ResourcePipeline,
        events::pipeline::ResourceRenamed,
        data
    );
}