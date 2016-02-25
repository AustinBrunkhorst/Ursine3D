#include "ResourceImporter.h"

#include "PassThroughProcessor.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class FBXImporter : public ResourceImporter
            {
                RESOURCE_IMPORTER;

            public:
                FBXImporter(void);
                ~FBXImporter(void);

            private:
                ResourceData::Handle Import(ResourceImportContext &context) override;
            } Meta(
                Enable,
                DisplayName( "FBXImporter" ),
                ResourceImporterConfig( 
                    ExtensionList { "fbx" },
                    typeof( ursine::rp::PassThroughProcessor )
                )
            );
        }
    }
}