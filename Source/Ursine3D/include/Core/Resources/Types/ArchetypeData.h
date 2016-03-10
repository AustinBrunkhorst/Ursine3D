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

            // Gets the cached archetype data for this resource
            const Json &GetData(void);

            // Instatiates the archetype represented in this resource
            // in the given world
            ecs::EntityHandle Instantiate(ecs::World *world) const;

        private:
            Json m_data;

            void Write(pipeline::ResourceWriter &output) override;

            meta::Type GetReaderType(void) override;
        } Meta(Register);
    }
}