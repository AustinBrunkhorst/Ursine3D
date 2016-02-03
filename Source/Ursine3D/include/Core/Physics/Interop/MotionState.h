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
			MotionState(BodyFlag bodyFlag);

        #ifdef BULLET_PHYSICS

			// Used by bullet for getting the user's position (kinematic and dynamic)
			void getWorldTransform(btTransform &centerOfMassWorldTrans) const override;

			// Used by bullet to set the user's position (dynamic only)
            void setWorldTransform(const btTransform &centerOfMassWorldTrans) override;

        #endif

            bool m_dirty;
			BodyFlag m_bodyFlag;
        };
    }
}
