/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BoxCollider.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "BoxCollider.h"

namespace ursine
{
    namespace physics
    {
        BoxCollider::BoxCollider(void)
        #ifdef BULLET_PHYSICS
            : BoxColliderBase( btVector3( 1.0f, 1.0f, 1.0f ) )
        #endif
        {

        }

        SVec3 BoxCollider::GetDimensions(void) const
        {
        #ifdef BULLET_PHYSICS
            
            auto dim = 2.0f * getHalfExtentsWithoutMargin( );

            return { dim.getX( ), dim.getY( ), dim.getZ( ) };

        #endif
        }

        void BoxCollider::SetDimensions(const SVec3 &dimensions)
        {
        #ifdef BULLET_PHYSICS
            
            auto dim = btVector3(
                dimensions.X( ),
                dimensions.Y( ),
                dimensions.Z( )
            );

            m_implicitShapeDimensions = 0.5f * dim;

        #endif
        }
    }
}
