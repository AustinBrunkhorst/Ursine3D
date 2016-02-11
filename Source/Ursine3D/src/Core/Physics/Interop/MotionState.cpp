/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** MotionState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "MotionState.h"

namespace ursine
{
    namespace physics
    {
        MotionState::MotionState(BodyFlag bodyFlag)
            : MotionStateBase( ) 
            , m_dirty( false )
		    , m_bodyFlag( bodyFlag ) { }

    #ifdef BULLET_PHYSICS

		void MotionState::getWorldTransform(btTransform &centerOfMassWorldTrans) const
        {
	        btDefaultMotionState::getWorldTransform( centerOfMassWorldTrans );
        }

        void MotionState::setWorldTransform(const btTransform& centerOfMassWorldTrans)
        {
		   if (m_bodyFlag == BF_KINEMATIC)
				return;

            btDefaultMotionState::setWorldTransform( centerOfMassWorldTrans );

            m_dirty = true;
        }

    #endif
    }
}
