/* ---------------------------------------------------------------------------
** Team Bear King
** 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SweptControllerSystem.cpp
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "SweptControllerSystem.h"

#include "SweptControllerComponent.h"

namespace ursine
{
	namespace ecs
	{
		ENTITY_SYSTEM_DEFINITION( SweptControllerSystem );

		SweptControllerSystem::SweptControllerSystem(World *world)
			: FilterSystem( world, Filter( ).One<SweptController>( ) )
		{
			
		}

		void SweptControllerSystem::Process(Entity* entity)
		{
			auto &c = *entity->GetComponent<SweptController>( );

			if (!c.m_active)
				return;

			float dt = Application::Instance->GetDeltaTime( );

			// TODO: Set the collision group for the detection query.
			// Only detection settings from the collision table have any effect.
			// this.CastFilter.CollisionGroup = this.Collider.CollisionGroup;

			// Removes any movement given along the worldUp axis.
			c.m_movementDirection = c.m_movementDirection - 
									SVec3::ProjectToNorm( c.m_movementDirection, c.m_worldUp );
			c.m_movementDirection.Normalize( );

			// Decompose velocity directions so that movement logic
			// can be independent of jumping / falling.
			auto verticalVelocity = SVec3::ProjectToNorm( c.m_controllerVelocity, c.m_worldUp );
			auto horizontalVelocity = c.m_controllerVelocity - verticalVelocity;

			// Get acceleration / deceleration types.
			auto acceleration = dt;
			auto deceleration = dt;

			if (c.m_grounded)
			{
				acceleration *= c.m_groundAcceleration * c.m_groundTraction;
				deceleration *= c.m_groundDeceleration * c.m_groundTraction;
			}
			else
			{
				acceleration *= c.m_airAcceleration * c.m_airTraction;
				deceleration *= c.m_airDeceleration * c.m_airTraction;
			}

			// Reset traction scalars for next update.
			c.m_groundTraction = 1.0f;
			c.m_airTraction = 1.0f;

			// Get velocity directions relative to input movement.
			// sideVelocity will be all of horizontalVelocity when movement is zero,
			// this will decelerate horizontalVelocity to zero when there is no input movement.
			auto forwardVelocity = SVec3::ProjectToNorm( horizontalVelocity, c.m_movementDirection );
			auto sideVelocity = horizontalVelocity - forwardVelocity;

			// Decelerate velocity that is not in the direction of movement.
			// Deceleration amount can ohnly take velocity to zero, not backwards.
			auto cappedSideDecel = math::Min( sideVelocity.Length( ), deceleration );
			horizontalVelocity -= SVec3::Normalize( sideVelocity ) * cappedSideDecel;

			{
				// cache the forwardVelocity length for the calculations ahead
				auto forwardVelocityLen = forwardVelocity.Length( );
				auto forwardVelocityLenInv = 1.0f / forwardVelocityLen;

				// If movement is against current velocity, apply deceleration to assist movement.
				if (forwardVelocity.Dot( c.m_movementDirection ) < 0.0f)
				{
					auto cappedForwardDecel = math::Min( forwardVelocityLen, deceleration );
					horizontalVelocity -= forwardVelocity * forwardVelocityLenInv * cappedForwardDecel;
				}
				// If movement is over max speed, decelerate it.
				else if (forwardVelocityLen > c.m_maxMoveSpeed)
				{
					auto cappedForwardDecel = math::Min( forwardVelocityLen - c.m_maxMoveSpeed, deceleration );
					horizontalVelocity -= forwardVelocity * forwardVelocityLenInv * cappedForwardDecel;
				}
			}

			// Accelerate inthe direction of movement, only up to the max speed.
			// This check is only so that the character movement cannot accelerate
			// beyond max speed, but other things could cause it to if desired.
			{
				auto horizontalVelocityLen2 = horizontalVelocity.LengthSquared( );
				if (horizontalVelocityLen2 < c.m_maxMoveSpeed * c.m_maxMoveSpeed)
				{
					auto cappedAccel = math::Min( c.m_maxMoveSpeed - sqrt( horizontalVelocityLen2 ), acceleration );
					horizontalVelocity += c.m_movementDirection * cappedAccel;
				}
			}

			if (c.m_grounded)
			{
				// Do not want to accumulate vertical velocity when grounded.
				// Gravity is effectively turned off while grounded.
				verticalVelocity = SVec3::Zero( );
			}
			else
			{
				// Apply gravity in opposite direction of worldUp.
				verticalVelocity -= c.m_worldUp * c.m_gravity * dt;

				// This will cap velocity in the downward direction only.
				// Condition can be removed to cap both directions, or removed entirely for no vertical speed cap.
				if (verticalVelocity.Dot( c.m_worldUp ) < 0.0f)
				{
					auto len = verticalVelocity.Length( );
					auto lenInv = 1.0f / len;

					auto cappedFallSpeed = math::Min( len, c.m_maxFallSpeed );
					verticalVelocity = verticalVelocity * lenInv * cappedFallSpeed;
				}
			}

			// Recompose velocity directions.
			c.m_controllerVelocity = horizontalVelocity + verticalVelocity;

			// Makes sure jumping flag is removed when velocity is not upwards
			// so that an upward jump can be maintained while in contact with the ground.
			// i.e. Jumping into a slope.
			if (c.m_controllerVelocity.Dot( c.m_worldUp ) <= 0.0f)
				c.m_jumping = false;

			// Does a "collide and slide" like behavior, starting with controller velocity.
			sweptCollision( c, c.m_controllerVelocity, dt, false );

			// Does a sweep for every kinematic object the character is in contact with
			// using the velocity of that object. (for moving platforms and such)
			for (auto *e : c.m_kinematicContacts)
			{
				// Objects stored are from previous frame, so make sure they are still valid.
				// Additionally, do not sweep with another player's velocity, nothing meaningful will come of that
				if (e && e->HasComponent<Rigidbody>( ) && !e->HasComponent<SweptController>( ))
				{
					auto rigidbody = e->GetComponent<Rigidbody>( );

					sweptCollision( c, rigidbody->GetVelocity( ), dt, true );
				}
			}

			// Done after the sweep to stay in contact with the ground when detected
			snapToGround( c );

			// Event and data management for the end of the update.
			sweptCompleted( c );

			// Reset the movement direction per frame.
			c.m_movementDirection = SVec3::Zero( );
		}

		void SweptControllerSystem::sweptCollision(SweptController& controller, const SVec3& sweptVelocity, float timeLeft, bool kinematic)
		{
			// TODO:
		}

		void SweptControllerSystem::sweptCompleted(SweptController& controller)
		{
			// TODO:
		}

		void SweptControllerSystem::snapToGround(SweptController &controller)
		{
			// TODO:
		}

		bool SweptControllerSystem::isGroundSurface(SweptController &controller, const SVec3 &normal)
		{
			auto theta = normal.Dot( controller.m_worldUp );
			theta = math::Clamp( theta, -1.0f ,1.0f );
			
			auto angle = acos( theta );

			return math::RadiansToDegrees( angle - controller.m_epsilon ) <= 
				   controller.m_maxGroundSlope;
		}

		bool SweptControllerSystem::isCeilingSurface(SweptController &controller, const SVec3& normal)
		{
			auto theta = normal.Dot( -controller.m_worldUp );
			theta = math::Clamp( theta, -1.0f ,1.0f );
			
			auto angle = acos( theta );

			return math::RadiansToDegrees( angle - controller.m_epsilon ) <= 
				   controller.m_maxCeilingSlope;
		}

		SVec3 SweptControllerSystem::skewProjection(SweptController &controller, const SVec3& velocity, const SVec3& direction, const SVec3& normal)
		{
			auto vDotn = velocity.Dot( normal );
			auto dDotn = direction.Dot( normal );

			// No intersection if direction and plane are parallel.
			// Will only hapen if slope properties are set to meaningless values.
			if (abs( dDotn ) < controller.m_epsilon)
				return SVec3::Zero( );

			return velocity + direction * -(vDotn / dDotn);
		}

		void SweptControllerSystem::updateKinematicList(SweptController &controller)
		{
			controller.m_kinematicContacts = controller.m_kinematicPending;
			controller.m_kinematicPending.clear( );
		}
	}
}
