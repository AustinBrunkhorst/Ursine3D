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
#include "DebugSystem.h"

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
                         ursine::physics::RaycastType type = physics::RAYCAST_CLOSEST_HIT, 
                         bool debugDraw = false, float drawDuration = 2.0f);

            void SetGravity(const SVec3 &gravity);
            SVec3 GetGravity(void) const;

        protected:
            void Process(Entity *entity) override;

        private:
            physics::Simulation m_simulation;
			physics::DebugDrawer m_debugDrawer;

            Filter m_collisionShapes;

            DebugSystem *m_debugSystem;

            void OnInitialize(void) override;
            void OnRemove(void) override;

            void OnAfterLoad(void) override;

            void onComponentAdded(EVENT_HANDLER(World));
            void onComponentRemoved(EVENT_HANDLER(World));

            void onUpdate(EVENT_HANDLER(World));

            void addCollider(Entity *entity, physics::ColliderBase *collider, bool emptyCollider = false);
            void removeCollider(Entity *entity);

        } Meta(Enable);
    }
}
