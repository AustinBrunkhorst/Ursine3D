#pragma once

#include "ResourceData.h"

namespace ursine
{
    namespace resources
    {
        class UIResourceData : public ResourceData
        {
            RESOURCE_DATA;

        public:
            UIResourceData(void);

        private:
            void Write(pipeline::ResourceWriter &output) override;

            meta::Type GetReaderType(void) override;
        } Meta(Register);
    }
}