/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** WorldData.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceData.h"

#include "World.h"

namespace ursine
{
    namespace resources
    {
        class WorldData : public ResourceData
        {
            RESOURCE_DATA(WorldData);

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