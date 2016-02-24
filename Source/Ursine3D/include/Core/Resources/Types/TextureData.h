#include "ResourceData.h"
#include "GfxAPIDefines.h"

namespace ursine
{
    namespace resources
    {
        class TextureData : public ResourceData
        {
            RESOURCE_DATA;

        public:
            TextureData(void *bytes, size_t size, unsigned width, unsigned height);
            ~TextureData(void);

            graphics::GfxHND GetTextureHandle(void) const;

        private:
            graphics::GfxHND m_textureHandle;

            void Write(pipeline::ResourceWriter &output) override;

            meta::Type GetReaderType(void) override;
        } Meta(Register);
    }
}