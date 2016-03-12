#pragma once

#include "ResourceImporter.h"

#include "PassThroughProcessor.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class UIProjectImporter : public ResourceImporter
            {
                RESOURCE_IMPORTER;

            public:
                UIProjectImporter(void);

            private:
                ResourceData::Handle Import(ResourceImportContext &context) override;
            } Meta(
                Enable,
                DisplayName( "UIProjectImporter" ),
                ResourceImporterConfig( 
                    ExtensionList { "uiproj" },
                    typeof( ursine::rp::PassThroughProcessor )
                ),
                ResourceSyncConfig(
                    FileExclusionList {
                        // whitelist non build/preprocess files
                        SYNC_EXPR_EXCLUDE_NOT( "js|html|css|png|svg|woff2" )
                    }
                )
            );
        }
    }
}