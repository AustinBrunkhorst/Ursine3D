/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SweptControllerComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "SweptControllerComponent.h"

#include "EntityEvent.h"
#include "CollisionEventArgs.h"

namespace ursine
{
	namespace ecs
	{
		NATIVE_COMPONENT_DEFINITION( SweptController );

		SweptController::SweptController(void)
			: BaseComponent( )
			, m_active( true )
			, m_forwardEvents( true )
			, m_grounded( true )
			, m_jumping( false )
			, m_worldUp( SVec3::UnitY( ) )
			, m_gravity( 10.0f )
			, m_jumpSpeed( 5.0f )
			, m_jumpCancelFactor( 0.5f )
			, m_maxMoveSpeed( 10.0f )
			, m_maxFallSpeed( 50.0f )
			, m_groundAcceleration( 50.0f )
			, m_airAcceleration( 10.0f )
			, m_groundDeceleration( 50.0f )
			, m_airDeceleration( 10.0f )
			, m_maxGroundSlope( 45.0f )
			, m_maxCeilingSlope( 45.0f )
			, m_groundSnapDistance( 0.1f )
			, m_groundTraction( 1.0f )
			, m_airTraction( 1.0f )
			, m_epsilon( 0.0001f )
		{
			
		}

		SweptController::~SweptController(void)
		{
			m_transform = nullptr;
			m_rigidbody = nullptr;
			m_collider = nullptr;

			GetOwner( )->Listener( this )
				.Off( ENTITY_COLLISION_PERSISTED, &SweptController::onCollision );
		}

		void SweptController::OnInitialize(void)
		{
			auto owner = GetOwner( );

			m_transform = owner->GetTransform( );
			m_rigidbody = owner->GetComponent<Rigidbody>( );
			m_collider = owner->GetComponent<CylinderCollider>( );

			// Set the rigidbody to kinematic (in case the user didn't)
			m_rigidbody->SetBodyFlag( BodyFlag::Kinematic );

			// TODO: Only want to resolve collision with static and kinematic.
			// this.CastFilter = this.Space.PhysicsSpace.CreateDefaultCastFilter();
			// this.CastFilter.IgnoreDynamic = true;
			// this.CastFilter.IgnoreGhost = true;
			// this.CastFilter.IgnoreKinematic = false;
			// this.CastFilter.IgnoreStatic = false;
			// this.CastFilter.IgnoreChildren = true;

			GetOwner( )->Listener( this )
				.On( ENTITY_COLLISION_PERSISTED, &SweptController::onCollision );
		}

		bool SweptController::GetActive(void) const
		{
			return m_active;
		}

		void SweptController::SetActive(bool active)
		{
			m_active = active;
		}

		const SVec3& SweptController::GetWorldUp(void) const
		{
			return m_worldUp;
		}

		void SweptController::SetWorldUp(const SVec3& up)
		{
			m_worldUp = SVec3::Normalize( up );
		}

		bool SweptController::GetForwardEvents(void) const
		{
			return m_forwardEvents;
		}

		void SweptController::SetForwardEvents(bool forward)
		{
			m_forwardEvents = forward;
		}

		float SweptController::GetGravity(void) const
		{
			return m_gravity;
		}

		void SweptController::SetGravity(float gravity)
		{
			m_gravity = gravity;
		}

		float SweptController::GetJumpSpeed(void) const
		{
			return m_jumpSpeed;
		}

		void SweptController::SetJumpSpeed(float jumpSpeed)
		{
			m_jumpSpeed = jumpSpeed;
		}

		float SweptController::GetJumpCancelFactor(void) const
		{
			return m_jumpCancelFactor;
		}

		void SweptController::SetJumpCancelFactor(float jumpCancelFactor)
		{
			m_jumpCancelFactor = jumpCancelFactor;
		}

		float SweptController::GetMaxMoveSpeed(void) const
		{
			return m_maxMoveSpeed;
		}

		void SweptController::SetMaxMoveSpeed(float maxMoveSpeed)
		{
			if (maxMoveSpeed < 0.0f)
				return;

			m_maxMoveSpeed = maxMoveSpeed;
		}

		float SweptController::GetMaxFallSpeed(void) const
		{
			return m_maxFallSpeed;
		}

		void SweptController::SetMaxFallSpeed(float maxFallSpeed)
		{
			if (maxFallSpeed < 0.0f)
				return;

			m_maxFallSpeed = maxFallSpeed;
		}

		float SweptController::GetGroundAcceleration(void) const
		{
			return m_groundAcceleration;
		}

		void SweptController::SetGroundAcceleration(float groundAcceleration)
		{
			m_groundAcceleration = groundAcceleration;
		}

		float SweptController::GetAirAcceleration(void) const
		{
			return m_airAcceleration;
		}

		void SweptController::SetAirAcceleration(float airAcceleration)
		{
			m_airAcceleration = airAcceleration;
		}

		float SweptController::GetGroundDeceleration(void) const
		{
			return m_groundDeceleration;
		}

		void SweptController::SetGroundDeceleration(float groundDeceleration)
		{
			m_groundDeceleration = groundDeceleration;
		}

		float SweptController::GetAirDeceleration(void) const
		{
			return m_airDeceleration;
		}

		void SweptController::SetAirDeceleration(float airDeceleration)
		{
			m_airDeceleration = airDeceleration;
		}

		float SweptController::GetMaxGroundSlope(void) const
		{
			return m_maxGroundSlope;
		}

		void SweptController::SetMaxGroundSlope(float groundSlope)
		{
			m_maxGroundSlope = groundSlope;
		}

		float SweptController::GetMaxCeilingSlope(void) const
		{
			return m_maxCeilingSlope;
		}

		void SweptController::SetMaxCeilingSlope(float ceilingSlope)
		{
			m_maxCeilingSlope = ceilingSlope;
		}

		float SweptController::GetGroundSnapDistance(void) const
		{
			return m_groundSnapDistance;
		}

		void SweptController::SetGroundSnapDistance(float snapDist)
		{
			if (snapDist < 0.0f)
				return;

			m_groundSnapDistance = snapDist;
		}

		bool SweptController::GetGrounded(void) const
		{
			return m_grounded;
		}

		bool SweptController::GetJumping(void) const
		{
			return m_jumping;
		}

		void SweptController::SetMovementDirection(const SVec3& movementDirection)
		{
			m_movementDirection = movementDirection;
		}

		const SVec3& SweptController::GetMovementDirection() const
		{
			return m_movementDirection;
		}

		void SweptController::Jump(void)
		{
			if (m_grounded)
			{
				// ControllerVelocity will not already have any velocity on the
				// WorldUp axis when grounded, can simply add jump velocity.
				m_controllerVelocity += m_worldUp * m_jumpSpeed;
				m_grounded = false;
				m_jumping = true;
			}
		}

		void SweptController::JumpUnconditionally(void)
		{
			// Remove any velocity that's currently on the worldUp axis first
			// and then add jump velocity.
			m_controllerVelocity -= SVec3::ProjectToNorm( m_controllerVelocity, m_worldUp );
			m_controllerVelocity += m_worldUp * m_jumpSpeed;
			m_grounded = false;
			m_jumping = true;
		}

		void SweptController::JumpDirectionally(const SVec3 &additionalVelocity)
		{
			m_controllerVelocity = additionalVelocity + m_worldUp * m_jumpSpeed;
			m_grounded = false;
			m_jumping = true;
		}

		void SweptController::JumpCancel(void)
		{
			// If a jump is cancelled while still moving upward,
			// this will reduce the remaining velocity to make a shorter jump.
			// Holding jump longer will make a higher jump.
			if (m_jumping && m_controllerVelocity.Dot( m_worldUp ) > 0.0f)
			{
				m_controllerVelocity -= 
					SVec3::ProjectToNorm( m_controllerVelocity, m_worldUp ) *
					m_jumpCancelFactor;
			}

			m_jumping = false;
		}

		float SweptController::GetGroundTraction(void) const
		{
			return m_groundTraction;
		}

		void SweptController::SetGroundTraction(float groundTraction)
		{
			m_groundTraction = groundTraction;
		}

		float SweptController::GetAirTraction(void) const
		{
			return m_airTraction;
		}

		void SweptController::SetAirTraction(float airTraction)
		{
			m_airTraction = airTraction;
		}

		// Forwards object to kinematic check
		void SweptController::onCollision(EVENT_HANDLER(Entity))
		{
			EVENT_ATTRS(Entity, physics::CollisionEventArgs);

			if (!skipResolution( /*this.Collider, event.OtherObject.Collider*/ ))
			{
			    addIfKinematic( args->otherEntity );
			}
		}

		void SweptController::addIfKinematic(const EntityHandle &entity)
		{
			auto *rigidbody = entity->GetComponent<Rigidbody>( );

			if (rigidbody && rigidbody->GetBodyFlag( ) == BodyFlag::Kinematic)
			{
				auto found = std::find( m_kinematicPending.begin( ), m_kinematicPending.end( ), entity );

				if (found == m_kinematicPending.end( ))
					m_kinematicPending.push_back( entity );
			}
		}

		bool SweptController::skipResolution(/*TODO: CollisionFilter a, CollisionFilter b*/)
		{
			// var collisionTable = this.Space.PhysicsSpace.CollisionTable;
			// var filter = collisionTable.FindFilter(colliderA.CollisionGroup, colliderB.CollisionGroup);
			// if (filter != null && filter.CollisionFlag == FilterCollisionFlags.SkipResolution)
			// {
			// 	return true;
			// }

			return false;
		}
	}
}
