#include "ResourceTypeReader.h"

namespace ursine
{
    namespace resources
    {
        class AnimationClipReader : public ResourceTypeReader
        {
            RESOURCE_TYPE_READER;

        public:
            AnimationClipReader(void);

        private:
            ResourceData::Handle Read(ResourceReader &input) override;
        } Meta(
            Enable,
            DisplayName( "AnimationClipReader" )
        );
    }
}