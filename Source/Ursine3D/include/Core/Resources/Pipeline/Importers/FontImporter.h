#pragma once

#include "ResourceImporter.h"

#include "PassThroughProcessor.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class FontImporter : public ResourceImporter
            {
                RESOURCE_IMPORTER;

            public:
                FontImporter(void);

            private:
                ResourceData::Handle Import(ResourceImportContext &context) override;
            } Meta(
                Enable,
                DisplayName( "FontImporter" ),
                ResourceImporterConfig( 
                    ExtensionList { 
                        "bmfc",
                    },
                    typeof( ursine::rp::PassThroughProcessor )
                )
            );
        }
    }
}