#include "UrsinePrecompiled.h"

#include "UIProjectImporter.h"
#include "UIProjectData.h"

#include "UIScreenData.h"

#include "ResourcePipelineManager.h"
#include "BuiltInResourceConfig.h"

#include <unordered_set>

namespace ursine
{
    namespace
    {
        void importEntry(
            const fs::path &sourcePath,
            const fs::path &entryPath, 
            resources::UIProjectData::ResourceTable &table
        );

        fs::path generateScreenResource(
            rp::ResourceItem::Handle project,
            const fs::path &sourcePath
        );
    }

    rp::UIProjectImporter::UIProjectImporter(void) { }

    resources::ResourceData::Handle rp::UIProjectImporter::Import(ResourceImportContext &context)
    {
        UIProjectData::ResourceTable table;

        auto directoryFileName = context.resource->GetSourceFileName( );

        auto *syncConfig = decltypeof( *this ).GetMeta( ).GetProperty<ResourceSyncConfig>( );

        UAssert( syncConfig,
            "UIProjectImporter must have a ResourceSyncConfig meta property."    
        );

        fs::RecursiveDirectoryIterator it( directoryFileName );
        fs::RecursiveDirectoryIterator itEnd;
        
        fs::PathSet screens;

        for (; it != itEnd; ++it)
        {
            auto &entry = it->path( );

            // ignore files that are excluded from syncing
            if (syncConfig->IsExcluded( entry.string( ) ))
                continue;

            auto relativePath = fs::MakeRelativePath( 
                directoryFileName, 
                entry 
            );

            importEntry( entry, relativePath, table );

            auto extension = entry.extension( ).string( ).substr( 1 );

            utils::MakeLowerCase( extension );

            if (extension == kResourceTypeUIScreenExtension)
                screens.emplace( entry );
        }

        // build the generated screen resources
        for (auto &screen : screens)
        {
            context.AllocateGeneratedResource( 
                generateScreenResource( context.resource, screen )
            );
        }

        return std::make_shared<UIProjectData>( std::move( table ) );
    }

    namespace
    {
        void importEntry(
            const fs::path &sourcePath,
            const fs::path &entryPath,
            resources::UIProjectData::ResourceTable &table
        )
        {
            auto data = std::make_shared<BinaryData>( );

            UAssert( fs::LoadAllBinary( sourcePath.string( ), *data ),
                "Failed to load UI resource entry.\nfile: %s",
                sourcePath.string( ).c_str( )
            );

            table.emplace( entryPath, data );
        }

        fs::path generateScreenResource(
            rp::ResourceItem::Handle project,
            const fs::path &sourcePath
        )
        {
            auto screenName = sourcePath.stem( );
            auto parentDirectory = project->GetSourceFileName( );

            auto outputFile = change_extension( 
                parentDirectory.parent_path( ) / screenName,
                rp::kResourceTypeUIScreenExtension 
            );

            auto relativePath = fs::MakeRelativePath(
                parentDirectory,
                sourcePath
            );

            Json screenData = Json::object {
                { "project", to_string( project->GetGUID( ) ) },
                { "path", relativePath.string( ) }
            };

            UAssertCatchable( fs::WriteAllText( outputFile.string( ), screenData.dump( true ) ),
                "Unable to generate screen resource.\nfile: %s",
                outputFile.string( ).c_str( )
            );

            return outputFile;
        }
    }
}