#include "ResourceData.h"

namespace ursine
{
    namespace resources
    {
        class TextureData : public ResourceData
        {
            RESOURCE_DATA;

        public:
            TextureData(void) = default;

        private:
            void Write(pipeline::ResourceWriter &output) override;

            meta::Type GetReaderType(void) override;
        } Meta(Register);
    }
}