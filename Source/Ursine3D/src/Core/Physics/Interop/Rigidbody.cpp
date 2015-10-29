#include "UrsinePrecompiled.h"

#include "Rigidbody.h"

#include "TransformComponent.h"

namespace ursine
{
    namespace physics
    {
        Rigidbody::Rigidbody(float mass, ColliderBase *collider, BodyType bodyType)
            : RigidbodyBase( RigidbodyConstructionInfo(mass, nullptr, collider) )
            , m_gettingTransform( false )
            , m_mass( mass )
        {
        #ifdef BULLET_PHYSICS
            setMotionState( &m_motionState );
        #endif

            SetBodyType( bodyType );
        }

        void Rigidbody::SetBodyType(BodyType bodyType)
        {
            m_bodyType = bodyType;

        #ifdef BULLET_PHYSICS
            setCollisionFlags( m_bodyType );
        #endif

            if (bodyType == BODY_DYNAMIC)
                SetAwake( );
        }

        BodyType Rigidbody::GetBodyType(void) const
        {
            return m_bodyType;
        }

        void Rigidbody::SetTransform(ecs::Transform *transform)
        {
            if (m_gettingTransform)
                return;

        #ifdef BULLET_PHYSICS

            auto rot = transform->GetWorldRotation( );
            auto pos = transform->GetWorldPosition( );
            auto trans = btTransform(
                btQuaternion( rot.X( ), rot.Y( ), rot.Z( ), rot.W( ) ),
                btVector3( pos.X( ), pos.Y( ), pos.Z( ) )
            );

            setWorldTransform( trans );

        #endif

            if (m_bodyType == BODY_DYNAMIC)
                SetAwake( );
        }

        void Rigidbody::GetTransform(ecs::Transform *transform)
        {
            if (!m_motionState.m_dirty)
                return;

            // Setting this eleminates a circular get/set from the rigidbody component
            m_gettingTransform = true;

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

            m_gettingTransform = false;

            m_motionState.m_dirty = false;
        }

        void Rigidbody::SetCollider(ColliderBase* collider, bool emptyCollider)
        {
        #ifdef BULLET_PHYSICS
            setCollisionShape( collider );

            btVector3 localInertia;

            if (!emptyCollider)
                collider->calculateLocalInertia( m_mass, localInertia );

            setupRigidBody( RigidbodyConstructionInfo( 
                m_mass, &m_motionState, collider, localInertia 
            ) );
        #endif
        }

        ColliderBase *Rigidbody::GetCollider(void)
        {
        #ifdef BULLET_PHYSICS
            return getCollisionShape( );
        #endif
        }

        void Rigidbody::SetAwake(void)
        {
            if (m_gettingTransform)
                return;

        #ifdef BULLET_PHYSICS
            activate( );
        #endif
        }

    }
}
