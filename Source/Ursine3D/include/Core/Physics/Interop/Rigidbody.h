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
        class Rigidbody : public virtual RigidbodyBase
        {
        public:
            Rigidbody(float mass, ColliderBase *collider);

            void SetTransform(ecs::Transform *transform);

            void GetTransform(ecs::Transform *transform);

            void SetCollider(ColliderBase *collider);

            void RemoveCollider(void);

        private:
            MotionState m_motionState;
        };
    }
}
