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
                ~TextureImporter(void);

            private:
                SDL_Surface *m_inputSurface;

                ResourceData::Handle Import(const fs::path &fileName, const ResourceImportContext &context) override;

                bool BuildPreview(const fs::path &previewFileName, const Vec2 &maxDimension) override;
            } Meta(
                Enable,
                DisplayName( "TextureImporter" ),
                ResourceImporterConfig( 
                    ExtensionList { 
                        "bmp", "gif", "jpeg", "lbm",
                        "pcx", "png", "pnm", "tga",
                        "tiff", "webp", "xcf", "xpm", "xv"
                    },
                    typeof( ursine::rp::TextureProcessor )
                )
            );
        }
    }
}