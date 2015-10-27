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
            virtual void setWorldTransform(const btTransform &centerOfMassWorldTrans);
        #endif

            bool m_dirty;
        };
    }
}
