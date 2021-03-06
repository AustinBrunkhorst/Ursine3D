/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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
        class Rigidbody;
        class Body;
        class Ghost;

        class PhysicsSystem 
            : public EntitySystem
        {
            ENTITY_SYSTEM;

        public:
            friend class Rigidbody;

            Meta(Enable, DisableNonDynamic)
            PhysicsSystem(World *world);

            bool Raycast(const ursine::physics::RaycastInput &input, ursine::physics::RaycastOutput &output,
                         ursine::physics::RaycastType type = physics::RAYCAST_CLOSEST_HIT,
                         bool debugDraw = false, float drawDuration = 2.0f, Color color = Color::Blue, bool alwaysDrawLine = false);

            bool Sweep(Rigidbody *body, const SVec3 &velocity, float dt, 
                       ursine::physics::SweepOutput &output, ursine::physics::SweepType type, bool sorted = false);

            bool Sweep(Body *body, const SVec3 &velocity, float dt,
                       ursine::physics::SweepOutput &output, ursine::physics::SweepType type, bool sorted = false);

            bool Sweep(Ghost *body, const SVec3 &velocity, float dt,
                       ursine::physics::SweepOutput &output, ursine::physics::SweepType type, bool sorted = false);

            void SetGravity(const SVec3 &gravity);
            SVec3 GetGravity(void) const;

            void SetEnableDebugDraw(bool enable);
            bool GetEnableDebugDraw(void) const;

            void SetPlaymodeDebugDraw(bool enable);
            bool GetPlaymodeDebugDraw(void) const;

            void ClearContacts(Rigidbody *rigidbody);

        private:
            physics::Simulation m_simulation;
            physics::DebugDrawer m_debugDrawer;

            Filter m_collisionShapes;

            DebugSystem *m_debugSystem;

            bool m_enableDebugDraw;
            bool m_playmodeDebugDraw;

            std::vector<Rigidbody*> m_rigidbodies;
            std::vector<Body*> m_bodies;
            std::vector<Ghost*> m_ghosts;

            void OnInitialize(void) override;
            void OnRemove(void) override;

            void OnSceneReady(Scene *scene) override;

            void onComponentAdded(EVENT_HANDLER(World));
            void onComponentRemoved(EVENT_HANDLER(World));

            void onUpdate(EVENT_HANDLER(World));

            void addCollider(const EntityHandle &entity, physics::ColliderBase *collider, bool emptyCollider = false);
            void removeCollider(const EntityHandle &entity);

            bool sweep(physics::BodyBase *body, physics::ColliderBase *collider, const SVec3 &velocity, 
                       float dt, physics::SweepOutput &output, physics::SweepType type, bool sorted);

    #if defined(URSINE_WITH_EDITOR)

            void onEditorRender(EVENT_HANDLER(World));

    #endif

            void removeExistingCollider(const EntityHandle &entity, ComponentTypeID newCollider);

        } Meta(Enable, WhiteListMethods, AutoAddEntitySystem);
    }
}
