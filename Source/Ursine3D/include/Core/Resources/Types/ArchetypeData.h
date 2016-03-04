#pragma once

#include "ResourceData.h"

namespace ursine
{
    namespace resources
    {
        class ArchetypeData : public ResourceData
        {
            RESOURCE_DATA;

        public:
            ArchetypeData(const Json &archetypeJson);

            const Json &GetData(void);

        private:
            Json m_data;

            void Write(pipeline::ResourceWriter &output) override;

            meta::Type GetReaderType(void) override;
        } Meta(Register);
    }
}