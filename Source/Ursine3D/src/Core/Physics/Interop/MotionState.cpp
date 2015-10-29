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
