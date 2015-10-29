#pragma once

#include "PhysicsInteropConfig.h"

namespace ursine
{
    namespace physics
    {
        class ConeCollider : public ConeColliderBase
        {
        public:
            ConeCollider(void);

            float GetHeight(void) const;
            void SetHeight(float height);

            float GetRadius(void) const;
            void SetRadius(float radius);
        };
    }
}
