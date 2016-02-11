/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SphereCollider.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "SphereCollider.h"

namespace ursine
{
    namespace physics
    {
        SphereCollider::SphereCollider(void)
            : SphereColliderBase( 1.0f ) { }

        float SphereCollider::GetRadius(void) const
        {
        #ifdef BULLET_PHYSICS

            return getRadius( );

        #endif
        }

        void SphereCollider::SetRadius(float radius)
        {
        #ifdef BULLET_PHYSICS

            setUnscaledRadius( radius );

        #endif
        }
    }
}
