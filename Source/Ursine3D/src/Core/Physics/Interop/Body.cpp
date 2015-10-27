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
            auto pos = transform->GetWorldPosition( );
            
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
                SVec3( pos.getX( ), pos.getY( ), pos.getZ( ) )
            );

        #endif
        }

        void Body::SetCollider(ColliderBase* collider)
        {
            setCollisionShape( collider );
        }

        void Body::RemoveCollider(void)
        {
            setCollisionShape( nullptr );
        }
    }
}