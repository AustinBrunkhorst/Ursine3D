/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ConeCollider.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ConeCollider.h"

namespace ursine
{
    namespace physics
    {
        ConeCollider::ConeCollider(void)
        #ifdef BULLET_PHYSICS
            : ConeColliderBase( 0.5f, 1.0f )
        #endif
        {
            
        }

        float ConeCollider::GetHeight(void) const
        {
        #ifdef BULLET_PHYSICS

            return m_height;

        #endif
        }

        void ConeCollider::SetHeight(float height)
        {
        #ifdef BULLET_PHYSICS

            float radius_2 = GetRadius( ) * 2.0f;

            setLocalScaling( btVector3(
                radius_2, height, radius_2
            ) );

        #endif
        }

        float ConeCollider::GetRadius(void) const
        {
        #ifdef BULLET_PHYSICS

            return m_radius;

        #endif
        }

        void ConeCollider::SetRadius(float radius)
        {
        #ifdef BULLET_PHYSICS

            float radius_2 = radius * 2.0f;

            setLocalScaling( btVector3(
                radius_2, GetHeight( ), radius_2
            ) );

        #endif
        }
    }
}
