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
            , m_rotLock( 1 )
        {
        #ifdef BULLET_PHYSICS

            setMotionState( &m_motionState );

        #endif

            SetBodyType( bodyType );
        }

        void Rigidbody::SetID(int id)
        {
        #ifdef BULLET_PHYSICS

            setUserIndex( id );

        #endif
        }

        int Rigidbody::GetID(void)
        {
        #ifdef BULLET_PHYSICS

            return getUserIndex( );

        #endif
        }

        void Rigidbody::SetBodyType(BodyType bodyType)
        {
            m_bodyType = bodyType;

        #ifdef BULLET_PHYSICS

            if (bodyType != BODY_DYNAMIC)
                SetMass( 0.0f );
            else
                SetMass( m_mass );

        #endif

            if (bodyType == BODY_DYNAMIC)
            {
                SetGravity( m_gravity );
                SetAwake( );
            }
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
            auto pos = transform->GetWorldPosition( ) + rot * m_offset;
            auto trans = btTransform(
                btQuaternion( rot.X( ), rot.Y( ), rot.Z( ), rot.W( ) ),
                btVector3( pos.X( ), pos.Y( ), pos.Z( ) )
            );

            setWorldTransform( trans );

            setAngularFactor(
                btVector3( m_rotLock.X( ), m_rotLock.Y( ), m_rotLock.Z( ) )
            );

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
                SVec3( pos.getX( ), pos.getY( ), pos.getZ( ) ) - 
                transform->GetWorldRotation( ) * m_offset
            );

        #endif

            m_gettingTransform = false;

            m_motionState.m_dirty = false;
        }

        void Rigidbody::SetCollider(ColliderBase* collider, bool emptyCollider)
        {
        #ifdef BULLET_PHYSICS

            btVector3 localInertia( 0.0f, 0.0f, 0.0f );

            if (!emptyCollider)
                collider->calculateLocalInertia( m_mass, localInertia );

            m_localInertia.Set( 
                localInertia.getX( ), 
                localInertia.getY( ), 
                localInertia.getZ( ) 
            );

            setMotionState( &m_motionState );

            setCollisionShape( collider );
            
            setMassProps( GetMass( ), localInertia );

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

        void Rigidbody::SetOffset(const SVec3 &offset)
        {
            m_offset = offset;

            SetAwake( );
        }

        SVec3 Rigidbody::GetOffset(void) const
        {
            return m_offset;
        }

        void Rigidbody::LockXRotation(bool flag)
        {
            m_rotLock.X( ) = flag ? 0.0f : 1.0f;
        }

        void Rigidbody::LockYRotation(bool flag)
        {
            m_rotLock.Y( ) = flag ? 0.0f : 1.0f;
        }

        void Rigidbody::LockZRotation(bool flag)
        {
            m_rotLock.Z( ) = flag ? 0.0f : 1.0f;
        }

        void Rigidbody::UpdateInertiaTensor(void)
        {
        #ifdef BULLET_PHYSICS

            updateInertiaTensor( );

        #endif
        }

        void Rigidbody::SetGravity(const SVec3& gravity)
        {
            m_gravity = gravity;

        #ifdef BULLET_PHYSICS

            btVector3 vec( gravity.X( ), gravity.Y( ), gravity.Z( ) );
            
            setGravity( vec );

        #endif
        }

        void Rigidbody::SetMass(float mass)
        {
            m_mass = mass;

        #ifdef BULLET_PHYSICS

            btVector3 inertia( m_localInertia.X( ), m_localInertia.Y( ), m_localInertia.Z( ) );
            setMassProps( GetMass( ), inertia );

        #endif
        }

        float Rigidbody::GetMass(void) const
        {
            if (m_bodyType != BODY_DYNAMIC)
                return 0.0f;
            else
                return m_mass;
        }
    }
}
