#include "ResourceData.h"

namespace ursine
{
    namespace resources
    {
        class AnimationClipData : public ResourceData
        {
            RESOURCE_DATA;

        public:
            AnimationClipData(void);
            ~AnimationClipData(void);

        private:
            void Write(pipeline::ResourceWriter &output) override;

            meta::Type GetReaderType(void) override;
        } Meta(Register);
    }
}