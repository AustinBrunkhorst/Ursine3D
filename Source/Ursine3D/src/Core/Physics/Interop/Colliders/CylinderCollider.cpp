/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CylinderCollider.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "CylinderCollider.h"

namespace ursine
{
    namespace physics
    {
        CylinderCollider::CylinderCollider(void)
        #ifdef BULLET_PHYSICS
            : CylinderColliderBase( btVector3( 0.5f, 0.5f, 0.5f ) )
        #endif
        {
            
        }

        float CylinderCollider::GetHeight(void) const
        {
        #ifdef BULLET_PHYSICS

            return getImplicitShapeDimensions( ).getY( ) * 0.5f;

        #endif
        }

        void CylinderCollider::SetHeight(float height)
        {
        #ifdef BULLET_PHYSICS

            setImplicitShapeDimensions( btVector3( 
                GetRadius( ), height * 0.5f, GetRadius( )
            ) );

        #endif
        }

        float CylinderCollider::GetRadius(void) const
        {
        #ifdef BULLET_PHYSICS

            return getImplicitShapeDimensions( ).getX( );

        #endif
        }

        void CylinderCollider::SetRadius(float radius)
        {
        #ifdef BULLET_PHYSICS

            setImplicitShapeDimensions( btVector3( radius, GetHeight( ), radius) );

        #endif
        }
    }
}
