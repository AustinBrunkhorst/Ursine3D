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
