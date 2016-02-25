#include "ResourceImporter.h"

#include "PassThroughProcessor.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class AudioImporter : public ResourceImporter
            {
                RESOURCE_IMPORTER;

            public:
                AudioImporter(void);
                ~AudioImporter(void);

            private:
                ResourceData::Handle Import(ResourceImportContext &context) override;
            } Meta(
                Enable,
                DisplayName( "AudioImporter" ),
                ResourceImporterConfig( 
                    ExtensionList { "uaudio" },
                    typeof( ursine::rp::PassThroughProcessor )
                ),
                ResourceSyncConfig(
                    FileExclusionList {
                        "testing"
                    }
                )
            );
        }
    }
}