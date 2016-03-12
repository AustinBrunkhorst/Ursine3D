#include "UrsinePrecompiled.h"

#include "UIProjectImporter.h"
#include "UIProjectData.h"

namespace ursine
{
    namespace
    {
        void importEntry(
            const fs::path &sourcePath,
            const fs::path &entryPath, 
            resources::UIProjectData::ResourceTable &table
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
    }
}