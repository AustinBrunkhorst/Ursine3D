/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Body.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Body.h"
#include "TransformComponent.h"

namespace ursine
{
    namespace physics
    {
        Body::Body(void)
            : BodyBase( )
        {
            
        }

        void Body::SetTransform(ecs::Transform* transform)
        {
        #ifdef BULLET_PHYSICS

            auto rot = transform->GetWorldRotation( );
            auto pos = transform->GetWorldPosition( ) + rot * m_offset;
            
            setWorldTransform(btTransform(
                btQuaternion( rot.X( ), rot.Y( ), rot.Z( ), rot.W( ) ),
                btVector3( pos.X( ), pos.Y( ), pos.Z( ) )
            ));

        #endif
        }

        void Body::GetTransform(ecs::Transform* transform)
        {
        #ifdef BULLET_PHYSICS

            auto rot = m_worldTransform.getRotation( );
            auto pos = m_worldTransform.getOrigin( );

            transform->SetWorldRotation( 
                SQuat( rot.getX( ), rot.getY( ), rot.getZ( ), rot.getW( ) )
            );

            transform->SetWorldPosition(
                SVec3( pos.getX( ), pos.getY( ), pos.getZ( ) ) - 
                transform->GetWorldRotation( ) * m_offset
            );

        #endif
        }

        void Body::SetCollider(ColliderBase* collider)
        {
		#ifdef BULLET_PHYSICS
            setCollisionShape( collider );
		#endif
        }

        void Body::RemoveCollider(void)
        {
		#ifdef BULLET_PHYSICS
            setCollisionShape( nullptr );
		#endif
        }

        void Body::SetOffset(const SVec3 &offset)
        {
            m_offset = offset;
        }

        SVec3 Body::GetOffset(void) const
        {
            return m_offset;
        }
    }
}