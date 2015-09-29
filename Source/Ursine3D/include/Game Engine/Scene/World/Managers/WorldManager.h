/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WorldManager.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    namespace ecs
    {
        class World;

        class WorldManager
        {
            friend class World;

        protected:
            World *m_world;

            WorldManager(World *world);
            virtual ~WorldManager(void) {}

            // Called after the manager is added to its world.
            // This is before the world is actually serialized.
            virtual void OnInitialize(void) {}

            // Called when the owner world loads the attributes, entities, etc.
            virtual void OnWorldLoaded(void) {}
        };
    }
}
