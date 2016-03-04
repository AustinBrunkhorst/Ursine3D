#pragma once

#include "ResourceData.h"

namespace ursine
{
    namespace resources
    {
        class JsonData : public ResourceData
        {
            RESOURCE_DATA;

        public:
            JsonData(const Json &data);

            const Json &GetData(void) const;
        private:
            Json m_data;

            void Write(pipeline::ResourceWriter &output) override;

            meta::Type GetReaderType(void) override;
        } Meta(Register);
    }
}