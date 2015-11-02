#pragma once

#include "PhysicsInteropConfig.h"
#include "SVec3.h"

namespace ursine
{
    namespace physics
    {
        class BoxCollider : public BoxColliderBase
        {
        public:
            BoxCollider(void);

            SVec3 GetDimensions(void) const;
            void SetDimensions(const SVec3 &dimensions);
        };
    }
}
