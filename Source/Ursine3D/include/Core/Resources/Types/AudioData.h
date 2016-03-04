#pragma once

#include "ResourceData.h"

namespace ursine
{
    namespace resources
    {
        class AudioData : public ResourceData
        {
            RESOURCE_DATA;

        public:
            AudioData(void);
            ~AudioData(void);

        private:
            void Write(pipeline::ResourceWriter &output) override;

            meta::Type GetReaderType(void) override;
        } Meta(Register);
    }
}