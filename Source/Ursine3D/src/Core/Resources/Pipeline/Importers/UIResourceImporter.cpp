#include "UrsinePrecompiled.h"

#include "UIResourceImporter.h"
#include "UIResourceData.h"

namespace ursine
{
    namespace
    {
        void importEntry(
            const fs::path &sourcePath,
            const fs::path &entryPath, 
            resources::UIResourceData::ResourceTable &table
        );
    }

    rp::UIResourceImporter::UIResourceImporter(void) { }

    resources::ResourceData::Handle rp::UIResourceImporter::Import(ResourceImportContext &context)
    {
        UIResourceData::ResourceTable table;

        auto directoryFileName = context.resource->GetSourceFileName( );

        auto *syncConfig = decltypeof( *this ).GetMeta( ).GetProperty<ResourceSyncConfig>( );

        UAssert( syncConfig,
            "UIResourceImporter must have a ResourceSyncConfig meta property."    
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

        return std::make_shared<UIResourceData>( std::move( table ) );
    }

    namespace
    {
        void importEntry(
            const fs::path &sourcePath,
            const fs::path &entryPath,
            resources::UIResourceData::ResourceTable &table
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