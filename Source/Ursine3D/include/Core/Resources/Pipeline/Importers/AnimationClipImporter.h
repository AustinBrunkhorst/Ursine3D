#include "ResourceImporter.h"

#include "PassThroughProcessor.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class AnimationClipImporter : public ResourceImporter
            {
                RESOURCE_IMPORTER;

            public:
				AnimationClipImporter(void);
                ~AnimationClipImporter(void);

            private:
                ResourceData::Handle Import(const ResourceImportContext &context) override;
            } Meta(
                Enable,
                DisplayName( "AnimationClipImporter" ),
                ResourceImporterConfig( 
                    ExtensionList { "uanim" },
                    typeof( ursine::rp::PassThroughProcessor )
                )
            );
        }
    }
}