#include "UrsinePrecompiled.h"

#include "Rigidbody.h"

#include "TransformComponent.h"

namespace ursine
{
    namespace physics
    {
        Rigidbody::Rigidbody(float mass, ColliderBase *collider)
            : RigidbodyBase( RigidbodyConstructionInfo(mass, &m_motionState, collider) )
        {
            setMotionState( &m_motionState );
        }

        void Rigidbody::SetTransform(ecs::Transform *transform)
        {
        #ifdef BULLET_PHYSICS

            auto rot = transform->GetWorldRotation( );
            auto pos = transform->GetWorldPosition( );
            
            m_motionState.setWorldTransform(btTransform(
                btQuaternion( rot.X( ), rot.Y( ), rot.Z( ), rot.W( ) ),
                btVector3( pos.X( ), pos.Y( ), pos.Z( ) )
            ));

        #endif
        }

        void Rigidbody::GetTransform(ecs::Transform *transform)
        {
            if (!m_motionState.m_dirty)
                return;

        #ifdef BULLET_PHYSICS

            btTransform trans;

            m_motionState.getWorldTransform( trans );

            auto rot = trans.getRotation( );
            auto pos = trans.getOrigin( );

            transform->SetWorldRotation( 
                SQuat( rot.getX( ), rot.getY( ), rot.getZ( ), rot.getW( ) )
            );

            transform->SetWorldPosition(
                SVec3( pos.getX( ), pos.getY( ), pos.getZ( ) )
            );

        #endif

            m_motionState.m_dirty = false;
        }

        void Rigidbody::SetCollider(ColliderBase* collider)
        {
        #ifdef BULLET_PHYSICS
            setCollisionShape( collider );
        #endif
        }

        void Rigidbody::RemoveCollider(void)
        {
        #ifdef BULLET_PHYSICS
            setCollisionShape( nullptr );
        #endif    
        }

    }
}
