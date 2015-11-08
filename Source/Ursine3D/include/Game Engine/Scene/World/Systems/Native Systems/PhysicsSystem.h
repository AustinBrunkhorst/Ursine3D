/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PhysicsSystem.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "FilterSystem.h"
#include "Simulation.h"
#include "PhysicsInteropConfig.h"
#include "Raycasting.h"

namespace ursine
{
    namespace ecs
    {
        class PhysicsSystem 
            : public FilterSystem
        {
            ENTITY_SYSTEM;

        public:
            PhysicsSystem(World *world);

            bool Raycast(const ursine::physics::RaycastInput &input, 
                         ursine::physics::RaycastOutput &output,
                         ursine::physics::RaycastType type = physics::RAYCAST_CLOSEST_HIT);

        protected:
            void Process(Entity *entity) override;

        private:
            physics::Simulation m_simulation;
			physics::DebugDrawer m_debugDrawer;

            void OnInitialize(void) override;
            void OnRemove(void) override;

            void onComponentAdded(EVENT_HANDLER(World));
            void onComponentRemoved(EVENT_HANDLER(World));

            void onUpdate(EVENT_HANDLER(World));

            void addCollider(Entity *entity, physics::ColliderBase *collider, bool emptyCollider = false);
            void removeCollider(Entity *entity);

            Filter m_collisionShapes;

        } Meta(Enable);
    }
}
