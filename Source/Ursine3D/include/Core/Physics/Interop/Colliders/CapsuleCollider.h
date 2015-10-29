#pragma once

#include "PhysicsInteropConfig.h"

namespace ursine
{
    namespace physics
    {
        class CapsuleCollider : public CapsuleColliderBase
        {
        public:
            CapsuleCollider(void);

            float GetHeight(void) const;
            void SetHeight(float height);

            float GetRadius(void) const;
            void SetRadius(float radius);
        };
    }
}
