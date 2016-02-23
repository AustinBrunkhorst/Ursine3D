#include "ResourceData.h"

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

            uint8 *GetBytes(void) const;
            size_t GetSize(void) const;

            unsigned GetWidth(void) const;
            unsigned GetHeight(void) const;

        private:
            uint8 *m_bytes;
            size_t m_size;

            unsigned m_width;
            unsigned m_height;

            void Write(pipeline::ResourceWriter &output) override;

            meta::Type GetReaderType(void) override;
        } Meta(Register);
    }
}