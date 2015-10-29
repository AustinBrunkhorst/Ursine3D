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
            Rigidbody(float mass, ColliderBase *collider, BodyType bodyType = BODY_DYNAMIC);

            void SetBodyType(BodyType bodyType);
            
            BodyType GetBodyType(void) const;

            void SetTransform(ecs::Transform *transform);

            void GetTransform(ecs::Transform *transform);

            void SetCollider(ColliderBase *collider);

            void RemoveCollider(void);

            void SetAwake(void);

        private:
            MotionState m_motionState;

            BodyType m_bodyType;

            bool m_gettingTransform;

            float m_mass;
        };
    }
}
