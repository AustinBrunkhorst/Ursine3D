#pragma once

#include "PhysicsInteropConfig.h"

namespace ursine
{
    namespace physics
    {
        class SphereCollider : public SphereColliderBase
        {
        public:
            SphereCollider(void);

            float GetRadius(void) const;
            void SetRadius(float radius);
        };
    }
}