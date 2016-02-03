/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** MotionState.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

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
