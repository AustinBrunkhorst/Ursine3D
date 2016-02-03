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
        MotionState::MotionState(void)
            : MotionStateBase( ) 
            , m_dirty( false ) { }

    #ifdef BULLET_PHYSICS
        void MotionState::setWorldTransform(const btTransform& centerOfMassWorldTrans)
        {
            btDefaultMotionState::setWorldTransform( centerOfMassWorldTrans );

            m_dirty = true;
        }
    #endif
    }
}
