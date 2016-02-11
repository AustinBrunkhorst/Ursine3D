/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CapsuleCollider.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "CapsuleCollider.h"

namespace ursine
{
    namespace physics
    {
        CapsuleCollider::CapsuleCollider(void)
        #ifdef BULLET_PHYSICS
            : CapsuleColliderBase( 0.5f, 1.0f )
        #endif
        {
            
        }

        float CapsuleCollider::GetHeight(void) const
        {
        #ifdef BULLET_PHYSICS

            return getImplicitShapeDimensions( ).getY( );

        #endif
        }

        void CapsuleCollider::SetHeight(float height)
        {
        #ifdef BULLET_PHYSICS

            float radius = GetRadius( );

            setImplicitShapeDimensions( btVector3( 
                radius, height - 2.0f * radius, radius
            ) );

        #endif
        }

        float CapsuleCollider::GetRadius(void) const
        {
        #ifdef BULLET_PHYSICS

            return getImplicitShapeDimensions( ).getX( );

        #endif
        }

        void CapsuleCollider::SetRadius(float radius)
        {
        #ifdef BULLET_PHYSICS

            float height = GetHeight( );

            setImplicitShapeDimensions( btVector3( 
                radius, height, radius
            ) );

        #endif
        }
    }
}