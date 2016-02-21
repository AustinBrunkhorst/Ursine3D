#include "ResourceImporter.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class JsonImporter : public ResourceImporter
            {
                RESOURCE_IMPORTER;

            public:
                JsonImporter(void);

            private:
                ResourceData::Handle Import(const fs::path &fileName, const ResourceImportContext &context) override;
            } Meta(
                Enable,
                DisplayName( "JsonImporter" ),
                ResourceImporterConfig( 
                    ExtensionList { "json" },
                    typeof( ursine::rp::PassThroughProcessor )
                )
            );
        }
    }
}