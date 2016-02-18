#include "ResourceImporter.h"

#include "TextureProcessor.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class TextureImporter : public ResourceImporter
            {
                RESOURCE_IMPORTER;

            public:
                TextureImporter(void);

            private:
                ResourceData::Handle Import(const fs::path &fileName, const ResourceImportContext &context) override;
            } Meta(
                Enable,
                DisplayName( "TextureImporter" ),
                ResourceImporterConfig( 
                    ExtensionList { "png", "jpg", "jpeg", "dds" },
                    typeof( ursine::resources::pipeline::TextureProcessor )
                )
            );
        }
    }
}