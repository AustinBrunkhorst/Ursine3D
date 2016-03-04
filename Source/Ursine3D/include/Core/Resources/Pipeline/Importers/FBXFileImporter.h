#pragma once

#include "ResourceImporter.h"

#include "PassThroughProcessor.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class FBXFileImporter : public ResourceImporter
            {
                RESOURCE_IMPORTER;

            public:
                FBXFileImporter(void);
                ~FBXFileImporter(void);

            private:
                ResourceData::Handle Import(ResourceImportContext &context) override;
            } Meta(
                Enable,
                DisplayName( "FBXFileImporter" ),
                ResourceImporterConfig( 
                    ExtensionList { "fbx" },
                    typeof( ursine::rp::PassThroughProcessor )
                )
            );
        }
    }
}