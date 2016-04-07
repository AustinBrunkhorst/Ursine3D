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
        namespace project
        {
            const auto BuildStart = "ProjectBuildStart";
            const auto BuildProgress = "ProjectBuildProgress";
            const auto BuildComplete = "ProjectBuildComplete";

            const auto InstallStart = "ProjectInstallStart";
            const auto InstallComplete = "ProjectInstallComplete";
        }

        namespace pipeline
        {
            const auto ResourceBuildStart = "ResourceBuildStart";
            const auto ResourceBuildComplete = "ResourceBuildComplete";
            const auto ResourceBuildPreviewStart = "ResourceBuildPreviewStart";
            const auto ResourceBuildPreviewComplete = "ResourceBuildPreviewComplete";

            const auto ResourceAdded = "ResourceAdded";
            const auto ResourceModified = "ResourceModified";
            const auto ResourceRemoved = "ResourceRemoved";
            const auto ResourceRenamed = "ResourceRenamed";
        }
    }
}

EditorResourcePipelineManager::EditorResourcePipelineManager(Project *project)
    : m_editor( GetCoreSystem( Editor ) )
    , m_gameBuilder( project->GetGameBuilder( ) )
    , m_gameInstaller( project->GetGameInstaller( ) )
    , m_pipeline( project->GetResourcePipeline( ) )
{
    m_gameBuilder.Listener(this)
        .On( PGB_BUILD_START, &EditorResourcePipelineManager::onProjectBuildStart )
        .On( PGB_BUILD_PROGRESS, &EditorResourcePipelineManager::onProjectBuildProgress )
        .On( PGB_BUILD_COMPLETE, &EditorResourcePipelineManager::onProjectBuildComplete );    
    
    m_gameInstaller.Listener(this)
        .On( PIB_BUILD_START, &EditorResourcePipelineManager::onInstallStart )
        .On( PIB_BUILD_COMPLETE, &EditorResourcePipelineManager::onInstallComplete );

    m_pipeline.Listener( this )
        .On( rp::RP_BUILD_RESOURCE_START, &EditorResourcePipelineManager::onResourceBuildStart )
        .On( rp::RP_BUILD_RESOURCE_COMPLETE, &EditorResourcePipelineManager::onResourceBuildComplete )
        .On( rp::RP_RESOURCE_ADDED, &EditorResourcePipelineManager::onResourceAdded )
        .On( rp::RP_RESOURCE_MODIFIED, &EditorResourcePipelineManager::onResourceModified )
        .On( rp::RP_RESOURCE_REMOVED, &EditorResourcePipelineManager::onResourceRemoved )
        .On( rp::RP_RESOURCE_RENAMED, &EditorResourcePipelineManager::onResourceRenamed );
}

EditorResourcePipelineManager::~EditorResourcePipelineManager(void)
{
    m_pipeline.Listener( this )
        .Off( rp::RP_BUILD_RESOURCE_START, &EditorResourcePipelineManager::onResourceBuildStart )
        .Off( rp::RP_BUILD_RESOURCE_COMPLETE, &EditorResourcePipelineManager::onResourceBuildComplete )
        .Off( rp::RP_RESOURCE_ADDED, &EditorResourcePipelineManager::onResourceAdded )
        .Off( rp::RP_RESOURCE_MODIFIED, &EditorResourcePipelineManager::onResourceModified )
        .Off( rp::RP_RESOURCE_REMOVED, &EditorResourcePipelineManager::onResourceRemoved )
        .Off( rp::RP_RESOURCE_RENAMED, &EditorResourcePipelineManager::onResourceRenamed );

    m_gameInstaller.Listener(this)
        .Off( PIB_BUILD_START, &EditorResourcePipelineManager::onInstallStart )
        .Off( PIB_BUILD_COMPLETE, &EditorResourcePipelineManager::onInstallComplete );

    m_gameBuilder.Listener(this)
        .Off( PGB_BUILD_START, &EditorResourcePipelineManager::onProjectBuildStart )
        .Off( PGB_BUILD_PROGRESS, &EditorResourcePipelineManager::onProjectBuildProgress )
        .Off( PGB_BUILD_COMPLETE, &EditorResourcePipelineManager::onProjectBuildComplete );
}

void EditorResourcePipelineManager::onProjectBuildStart(EVENT_HANDLER(ProjectGameBuilder))
{
    m_editor->GetMainWindow( ).GetUI( )->Message(
        UI_CMD_BROADCAST,
        channel::ResourcePipeline,
        events::project::BuildStart,
        nullptr
    );
}

void EditorResourcePipelineManager::onProjectBuildProgress(EVENT_HANDLER(ProjectGameBuilder))
{
    EVENT_ATTRS(ProjectGameBuilder, ProjectGameBuildProgressArgs);

    Json data = Json::object {
        { "progress", args->progress }
    };

    m_editor->GetMainWindow( ).GetUI( )->Message(
        UI_CMD_BROADCAST,
        channel::ResourcePipeline,
        events::project::BuildProgress,
        data
    );
}

void EditorResourcePipelineManager::onProjectBuildComplete(EVENT_HANDLER(ProjectGameBuilder))
{
    EVENT_ATTRS(ProjectGameBuilder, ProjectGameBuildCompleteArgs);

    auto data = Json::object {
        { "successful", args->successful }
    };

    if (!args->successful)
        data[ "error" ] = args->error;

    m_editor->GetMainWindow( ).GetUI( )->Message(
        UI_CMD_BROADCAST,
        channel::ResourcePipeline,
        events::project::BuildComplete,
        data
    );
}

void EditorResourcePipelineManager::onInstallStart(EVENT_HANDLER(ProjectGameInstaller))
{
    m_editor->GetMainWindow( ).GetUI( )->Message(
        UI_CMD_BROADCAST,
        channel::ResourcePipeline,
        events::project::InstallStart,
        nullptr
    );
}

void EditorResourcePipelineManager::onInstallComplete(EVENT_HANDLER(ProjectGameInstaller))
{
    EVENT_ATTRS(ProjectGameInstaller, ProjectGameInstallerBuildCompleteArgs);

    auto data = Json::object {
        { "successful", args->successful }
    };

    if (!args->successful)
        data[ "error" ] = args->error;

    m_editor->GetMainWindow( ).GetUI( )->Message(
        UI_CMD_BROADCAST,
        channel::ResourcePipeline,
        events::project::InstallComplete,
        data
    );
}

void EditorResourcePipelineManager::onResourceBuildStart(EVENT_HANDLER(rp::ResourcePipelineManager))
{
    EVENT_ATTRS(rp::ResourcePipelineManager, rp::ResourceBuildArgs);

    Json data = Json::object {
        { "guid", to_string( args->resource->GetGUID( ) ) }
    };

    m_editor->GetMainWindow( ).GetUI( )->Message(
        UI_CMD_BROADCAST,
        channel::ResourcePipeline,
        events::pipeline::ResourceBuildStart,
        data
    );
}

void EditorResourcePipelineManager::onResourceBuildComplete(EVENT_HANDLER(rp::ResourcePipelineManager))
{
    EVENT_ATTRS(rp::ResourcePipelineManager, rp::ResourceBuildArgs);

    auto data = Json::object {
        { "guid", to_string( args->resource->GetGUID( ) ) },
        { "duration", args->operationDuration.Milliseconds( ) },
        { "successful", args->successful }
    };

    if (!args->successful)
    {
        data[ "error" ] = Json::object {
            { "message", args->error.GetErrorMessage( ) },
            { "file", args->error.GetFile( ) },
            { "function", args->error.GetFunction( ) },
            { "line", static_cast<int>( args->error.GetLine( ) ) }
        };
    }

    m_editor->GetMainWindow( ).GetUI( )->Message(
        UI_CMD_BROADCAST,
        channel::ResourcePipeline,
        events::pipeline::ResourceBuildComplete,
        data
    );
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