#include "UrsinePrecompiled.h"

#include "SphereColliderComponent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( SphereCollider );

        SphereCollider::SphereCollider(void)
            : BaseComponent( ) { }

        float SphereCollider::GetRadius(void) const
        {
            return m_sphereCollider.GetRadius( );
        }

        void SphereCollider::SetRadius(float radius)
        {
            m_sphereCollider.SetRadius( radius );
        }
    }
}
