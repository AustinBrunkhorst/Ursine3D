#pragma once

#include "ResourceImporter.h"

#include "PassThroughProcessor.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class UIResourceImporter : public ResourceImporter
            {
                RESOURCE_IMPORTER;

            public:
                UIResourceImporter(void);

            private:
                ResourceData::Handle Import(ResourceImportContext &context) override;
            } Meta(
                Enable,
                DisplayName( "UIResourceImporter" ),
                ResourceImporterConfig( 
                    ExtensionList { "ui" },
                    typeof( ursine::rp::PassThroughProcessor )
                ),
                ResourceSyncConfig(
                    FileExclusionList {
                        // whitelist non build/preprocess files
                        SYNC_EXPR_ALL_EXTENSIONS( "js|html|css|png" )
                    }
                )
            );
        }
    }
}