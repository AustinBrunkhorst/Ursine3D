#include "ResourceTypeReader.h"

namespace ursine
{
    namespace resources
    {
        class AudioReader : public ResourceTypeReader
        {
            RESOURCE_TYPE_READER;

        public:
            AudioReader(void);

        private:
            ResourceData::Handle Read(ResourceReader &input) override;
        } Meta(
            Enable,
            DisplayName( "AudioReader" )
        );
    }
}