#include "ResourceData.h"

#include "World.h"

namespace ursine
{
    namespace resources
    {
        class WorldData : public ResourceData
        {
            RESOURCE_DATA;

        public:
            WorldData(const Json &worldJson);

            ecs::World::Handle GetData(void);

        private:
            ecs::World::Handle m_data;

            void Write(pipeline::ResourceWriter &output) override;

            meta::Type GetReaderType(void) override;
        } Meta(Register);
    }
}