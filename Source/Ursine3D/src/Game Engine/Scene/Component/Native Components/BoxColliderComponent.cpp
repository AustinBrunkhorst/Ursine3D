#include "UrsinePrecompiled.h"

#include "BoxColliderComponent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( BoxCollider );

        BoxCollider::BoxCollider(void)
            : BaseComponent( ) { }

        SVec3 BoxCollider::GetDimensions(void) const
        {
            return m_boxCollider.GetDimensions( );
        }

        void BoxCollider::SetDimensions(const SVec3 &dimensions)
        {
            m_boxCollider.SetDimensions( dimensions );
        }
    }
}
