/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Rigidbody.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "PhysicsInteropConfig.h"
#include "MotionState.h"

namespace ursine
{
    namespace ecs
    {
        class Transform;
    }

    namespace physics
    {
        class Simulation;

        enum BodyType
        {
        #ifdef BULLET_PHYSICS
            BODY_DYNAMIC = 0,
            BODY_KINEMATIC = btRigidBody::CF_KINEMATIC_OBJECT,
            BODY_STATIC = btRigidBody::CF_STATIC_OBJECT
        #endif
        };

        class Rigidbody : public virtual RigidbodyBase
        {
        public:
            Rigidbody(float mass, ColliderBase *collider, BodyType bodyType = BODY_STATIC);

            void SetSimulation(Simulation *simulation);
            Simulation *GetSimulation(void);

            void SetID(int id);
            int GetID(void);

            void SetBodyType(BodyType bodyType);
            BodyType GetBodyType(void) const;

            void SetTransform(ecs::Transform *transform);
            void GetTransform(ecs::Transform *transform);

            void SetCollider(ColliderBase *collider, bool emptyCollider = false);
            ColliderBase *GetCollider(void);

            void SetAwake(void);

            void SetOffset(const SVec3 &offset);
            SVec3 GetOffset(void) const;

            void LockXRotation(bool flag);
            void LockYRotation(bool flag);
            void LockZRotation(bool flag);

            void UpdateInertiaTensor(void);

            void SetGravity(const SVec3 &gravity);

            void SetMass(float mass);
            float GetMass(void) const;

        private:
            BodyType m_bodyType;

            bool m_gettingTransform;

            bool m_emptyCollider;

            Simulation *m_simulation;

            float m_mass;

            SVec3 m_offset;

            SVec3 m_rotLock;

            SVec3 m_gravity;

            MotionState m_motionState;

            void addToSimulation(void);
            void removeFromSimulation(void);

        };
    }
}
