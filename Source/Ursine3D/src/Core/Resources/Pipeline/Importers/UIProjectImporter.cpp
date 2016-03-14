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
            rp::ResourceItem::Handle parent,
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
        
        // cached screens that were generated
        fs::PathSet cachedScreens;

        // screens found in the project
        fs::PathSet projectScreens;

        // screens that were added and need to be generated
        fs::PathSet addedScreens;

        // screens that existed in the build cache, but 
        // are no longer in the project
        fs::PathSet removedScreens;

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
                projectScreens.emplace( entry );
        }

        auto &generated = context.resource->GetBuildCache( ).generatedResources;

        // collect all generated UIScreens
        for (auto &guid : generated)
        {
            auto item = context.pipeline->GetItem( guid );

            if (item && item->GetBuildCache( ).processedType == typeof( UIScreenData ))
                cachedScreens.emplace( item->GetSourceFileName( ) );
        }

        // collect added screens and acknowledge existing ones
        for (auto &screen : projectScreens)
        {
            // exists in the cache, so remove it --
            // the screens remaining in "cachedScreens"
            // are assumed to be removed after this loop
            if (cachedScreens.count( screen ))
                cachedScreens.erase( screen );
            else
                addedScreens.emplace( screen );
        }

        // build the generated screen resources
        for (auto &screen : addedScreens)
        {
            context.AllocateGeneratedResource( 
                generateScreenResource( context.resource, screen ) 
            );
        }

        // remove cached screens that don't exist anymore
        for (auto &screen : cachedScreens)
        {
            context.pipeline->RemoveItem( 
                context.pipeline->GetItem( screen ) 
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
            rp::ResourceItem::Handle parent,
            const fs::path &sourcePath
        )
        {
            auto screenName = sourcePath.stem( );
            auto parentDirectory = parent->GetSourceFileName( );

            auto outputFile = change_extension( 
                parentDirectory.parent_path( ) / screenName,
                rp::kResourceTypeUIScreenExtension 
            );

            auto relativePath = fs::MakeRelativePath(
                parentDirectory,
                sourcePath
            );

            Json screenData = Json::object {
                { "parent", to_string( parent->GetGUID( ) ) },
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