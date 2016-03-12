#pragma once

#include "ResourceData.h"

namespace ursine
{
    namespace resources
    {
        class UIScreenData : public ResourceData
        {
            RESOURCE_DATA;

        public:
            UIScreenData(void);

            void Write(pipeline::ResourceWriter &output) override;

        private:
            meta::Type GetReaderType(void) override;
        } Meta( Register );
    }
}