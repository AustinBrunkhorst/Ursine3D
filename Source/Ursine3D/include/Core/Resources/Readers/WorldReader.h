#include "ResourceTypeReader.h"

namespace ursine
{
    namespace resources
    {
        class TextureReader : public ResourceTypeReader
        {
            RESOURCE_TYPE_READER;

        public:
            TextureReader(void);

        private:
            ResourceData::Handle Read(ResourceReader &input) override;
        } Meta(
            Enable,
            DisplayName( "TextureReader" )
        );
    }
}