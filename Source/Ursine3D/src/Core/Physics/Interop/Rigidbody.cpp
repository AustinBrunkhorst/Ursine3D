/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Rigidbody.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Rigidbody.h"
#include "Simulation.h"
#include "TransformComponent.h"

namespace ursine
{
    namespace physics
    {
        Rigidbody::Rigidbody(float mass, ColliderBase *collider, BodyFlag bodyType)
            : RigidbodyBase( RigidbodyConstructionInfo(mass, nullptr, collider) )
            , m_gettingTransform( false )
            , m_mass( mass )
            , m_rotLock( 1 )
            , m_simulation( nullptr )
            , m_emptyCollider( true )
            , m_enableSleeping( true )
			, m_ghost( false )
			, m_continuousCollisionDetection( false )
			, m_motionState( bodyType )
        {
        #ifdef BULLET_PHYSICS

            setMotionState( &m_motionState );

            setLinearFactor( btVector3( 1.0f, 1.0f, 1.0f ) );

            updateRotationFreeze( );

        #endif

            SetBodyFlag( bodyType );
			SetSleepToggle( m_enableSleeping );
        }

        void Rigidbody::SetSimulation(Simulation *simulation)
        {
            m_simulation = simulation;
        }

        Simulation *Rigidbody::GetSimulation(void)
        {
            return m_simulation;
        }

        void Rigidbody::SetUserID(int id)
        {
        #ifdef BULLET_PHYSICS

            setUserIndex( id );

        #endif
        }

	    int Rigidbody::GetUserID(void)
        {
        #ifdef BULLET_PHYSICS

            return getUserIndex( );

        #endif
        }

		void Rigidbody::SetUserPointer(void* ptr)
		{
		#ifdef BULLET_PHYSICS

			setUserPointer( ptr );

		#endif
		}

		void *Rigidbody::GetUserPointer(void)
		{
		#ifdef BULLET_PHYSICS

			return getUserPointer( );

		#endif
		}

		Rigidbody *Rigidbody::DownCast(BodyBase* body)
		{
		#ifdef BULLET_PHYSICS

			if (!body || body->getInternalType( ) != BT_RIGID_BODY)
				return nullptr;

			return reinterpret_cast<Rigidbody*>( body );

		#endif
		}

		const Rigidbody* Rigidbody::DownCast(const BodyBase* body)
		{
		#ifdef BULLET_PHYSICS

			if (!body || body->getInternalType() != BT_RIGID_BODY)
				return nullptr;

			return reinterpret_cast<const Rigidbody*>( body );

		#endif
		}

        void Rigidbody::SetBodyFlag(BodyFlag bodyFlag)
        {
            m_bodyType = bodyFlag;

        #ifdef BULLET_PHYSICS

            if (m_simulation && (bodyFlag == BF_STATIC || bodyFlag == BF_KINEMATIC))
                m_simulation->ClearContacts( *this );

            SetMass( m_mass );

			m_motionState.m_bodyFlag = bodyFlag;

        #endif

            if (bodyFlag == BF_DYNAMIC)
            {
                SetGravity( m_gravity );
                SetAwake( );
            }
        }

        BodyFlag Rigidbody::GetBodyFlag(void) const
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

			// Set the graphics world transform directly (this is because of kinematic bodies)
			m_motionState.m_graphicsWorldTrans = trans * m_motionState.m_centerOfMassOffset;

        #endif

            if (m_bodyType != BF_STATIC)
			{
                SetAwake( );
			}

			updateCCD( );
        }

        void Rigidbody::GetTransform(ecs::Transform *transform)
        {
            if (!m_motionState.m_dirty)
                return;

            // Setting this eleminates a circular get/set from the rigidbody component
            m_gettingTransform = true;

        #ifdef BULLET_PHYSICS

			btTransform trans;
			
			if (m_bodyType == BF_STATIC)
			{
				trans = getWorldTransform( );
			}
			else
			{
				m_motionState.getWorldTransform( trans );
			}

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
            m_emptyCollider = emptyCollider;

        #ifdef BULLET_PHYSICS

            btVector3 localInertia( 0.0f, 0.0f, 0.0f );

            if (!m_emptyCollider)
                collider->calculateLocalInertia( GetMass( ), localInertia );

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

        void Rigidbody::SetRotationFreezeX(bool flag)
        {
            m_rotLock.X( ) = flag ? 0.0f : 1.0f;
            
            updateRotationFreeze( );
        }

        bool Rigidbody::GetRotationFreezeX(void) const
        {
            return m_rotLock.X( ) == 0.0f;
        }

        void Rigidbody::SetRotationFreezeY(bool flag)
        {
            m_rotLock.Y( ) = flag ? 0.0f : 1.0f;

            updateRotationFreeze( );
        }

        bool Rigidbody::GetRotationFreezeY(void) const
        {
            return m_rotLock.Y( ) == 0.0f;
        }

        void Rigidbody::SetRotationFreezeZ(bool flag)
        {
            m_rotLock.Z( ) = flag ? 0.0f : 1.0f;

            updateRotationFreeze( );
        }

        void Rigidbody::SetSleepToggle(bool flag)
        {
            if (flag != m_enableSleeping)
            {
                m_enableSleeping = flag;

            #ifdef BULLET_PHYSICS

                if (m_enableSleeping)
                {
                    if (getActivationState( ) != ACTIVE_TAG)
                        setActivationState( ACTIVE_TAG );
                }
                else
                {
                    if (getActivationState( ) != DISABLE_DEACTIVATION)
                        setActivationState( DISABLE_DEACTIVATION );
                }

            #endif
            }
        }

        bool Rigidbody::GetSleepToggle(void) const
        {
            return m_enableSleeping;
        }

        bool Rigidbody::GetRotationFreezeZ(void) const
        {
            return m_rotLock.Z( ) == 0.0f;
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

	    const SVec3& Rigidbody::GetGravity(void) const
	    {
			return m_gravity;
	    }

	    void Rigidbody::SetMass(float mass)
        {
            m_mass = mass;

        #ifdef BULLET_PHYSICS

            removeFromSimulation( );

            btVector3 inertia( 0.0f, 0.0f, 0.0f );

            auto shape = getCollisionShape( );

            if (shape && !m_emptyCollider)
                shape->calculateLocalInertia( GetMass( ), inertia );

            setMassProps( GetMass( ), inertia );

            addToSimulation( );

        #endif

            if (m_bodyType == BF_DYNAMIC)
            {
                SetGravity( m_gravity );
                SetAwake( );
            }
        }

        float Rigidbody::GetMass(void) const
        {
            if (m_bodyType != BF_DYNAMIC)
                return 0.0f;
            else
                return m_mass;
        }

        void Rigidbody::SetVelocity(const SVec3 &velocity)
        {
        #ifdef BULLET_PHYSICS

            btVector3 vel( velocity.X( ), velocity.Y( ), velocity.Z( ) );

            setLinearVelocity( vel );

        #endif
        }

        SVec3 Rigidbody::GetVelocity(void) const
        {
            SVec3 vel;

        #ifdef BULLET_PHYSICS

            auto velocity = getLinearVelocity( );

            vel.Set(
                velocity.getX( ),
                velocity.getY( ),
                velocity.getZ( )
            );

        #endif

            return vel;
        }

        void Rigidbody::SetAngularVelocity(const SVec3& angularVelocity)
        {
        #ifdef BULLET_PHYSICS

            btVector3 angVel( angularVelocity.X( ), angularVelocity.Y( ), angularVelocity.Z( ) );

            setAngularVelocity( angVel );

        #endif
        }

	    SVec3 Rigidbody::GetAngularVelocity(void) const
        {
            SVec3 angVel;

        #ifdef BULLET_PHYSICS

            auto angularVel = getAngularVelocity( );

            angVel.Set(
                angularVel.getX( ),
                angularVel.getY( ),
                angularVel.getZ( )
            );

        #endif

            return angVel;
        }

		void Rigidbody::SetGhost(bool enable)
		{
			m_ghost = enable;

			if (m_ghost)
			{
			#ifdef BULLET_PHYSICS

				setCollisionFlags( getCollisionFlags( ) | CF_NO_CONTACT_RESPONSE );

			#endif
			}
			else
			{
			#ifdef BULLET_PHYSICS

				setCollisionFlags( getCollisionFlags( ) & ~CF_NO_CONTACT_RESPONSE );

			#endif
			}
		}

	    bool Rigidbody::GetGhost(void) const
		{
			return m_ghost;
		}

		void Rigidbody::SetContinuousCollisionDetection(bool enable)
		{
			m_continuousCollisionDetection = enable;

			updateCCD( );
		}

		bool Rigidbody::GetContinuousCollisionDetection(void) const
		{
			return m_continuousCollisionDetection;
		}

        void Rigidbody::AddForce(const SVec3& force)
        {
        #ifdef BULLET_PHYSICS

            btVector3 btForce( force.X( ), force.Y( ), force.Z( ) );

            applyCentralForce( btForce );

        #endif
        }

        void Rigidbody::AddForceRelative(const SVec3& force, ecs::Transform* transform)
        {
        #ifdef BULLET_PHYSICS

            btVector3 btForce( force.X( ), force.Y( ), force.Z( ) );

            applyCentralForce( btForce );

        #endif
        }

        void Rigidbody::AddForceAtPosition(const SVec3& force, const SVec3& worldPosition, ecs::Transform *transform)
        {
            auto localP = transform->ToLocal( worldPosition );

        #ifdef BULLET_PHYSICS

            btVector3 btForce( force.X( ), force.Y( ), force.Z( ) );
            btVector3 btRel( localP.X( ), localP.Y( ), localP.Z( ) );

            applyForce( btForce, btRel );

        #endif
        }

        void Rigidbody::AddImpulse(const SVec3& impulse)
        {
        #ifdef BULLET_PHYSICS

            btVector3 btImpulse(impulse.X( ), impulse.Y( ), impulse.Z( ));

            applyCentralImpulse(btImpulse);

        #endif
        }

        void Rigidbody::AddTorque(const SVec3& torque)
        {
        #ifdef BULLET_PHYSICS

            btVector3 btTorque( torque.X( ), torque.Y( ), torque.Z( ) );
            
            applyTorque( btTorque );

        #endif
        }

        void Rigidbody::AddTorqueRelative(const SVec3& torque, ecs::Transform* transform)
        {
        #ifdef BULLET_PHYSICS

            btVector3 btTorque( torque.X( ), torque.Y( ), torque.Z( ) );
            
            applyTorque( btTorque );

        #endif
        }

        void Rigidbody::addToSimulation(void)
        {
            if (m_simulation)
                m_simulation->AddRigidbody( this );
        }

        void Rigidbody::removeFromSimulation(void)
        {
            if (m_simulation)
                m_simulation->RemoveRigidbody( this );
        }

        void Rigidbody::updateRotationFreeze(void)
        {
        #ifdef BULLET_PHYSICS

            setAngularFactor(
                btVector3( m_rotLock.X( ), m_rotLock.Y( ), m_rotLock.Z( ) )
            );

        #endif
        }

		void Rigidbody::updateCCD(void)
        {
		#ifdef BULLET_PHYSICS

			if (m_continuousCollisionDetection)
			{
				// get the maximum scale factor
				btVector3 min, max;

                auto shape = getCollisionShape( );

                if (!shape)
                    return;

                shape->getAabb( getWorldTransform( ), min, max );

				float maxExtent = math::Max( 
					math::Max(
						max.x( ) - min.x( ), max.y( ) - min.y( ) 
					), max.z( ) - min.z( )
				);

				setCcdMotionThreshold( maxExtent );
				setCcdSweptSphereRadius( maxExtent * 0.2f );
			}
			else
			{
				setCcdMotionThreshold( 0.0f );
				setCcdSweptSphereRadius( 0.0f );
			}

		#endif
        }
    }
}
