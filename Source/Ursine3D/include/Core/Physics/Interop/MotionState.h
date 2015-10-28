#pragma once

#include "PhysicsInteropConfig.h"

namespace ursine
{
    namespace physics
    {
        class MotionState : public MotionStateBase
        {
        public:
            MotionState(void);

        #ifdef BULLET_PHYSICS
            void setWorldTransform(const btTransform &centerOfMassWorldTrans) override;
        #endif

            bool m_dirty;
        };
    }
}
