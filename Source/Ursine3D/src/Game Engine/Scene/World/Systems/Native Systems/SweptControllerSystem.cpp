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
#include "PhysicsSystem.h"

namespace ursine
{
	namespace ecs
	{
		ENTITY_SYSTEM_DEFINITION( SweptControllerSystem );

		SweptControllerSystem::SweptControllerSystem(World *world)
			: FilterSystem( world, Filter( ).One<SweptController>( ) )
		{
			
		}

		SweptControllerSystem::~SweptControllerSystem(void)
		{
			m_physics = nullptr;
		}

		void SweptControllerSystem::Initialize(void)
		{
			m_physics = m_world->GetEntitySystem<PhysicsSystem>( );
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

			if (c.m_movementDirection != SVec3::Zero( ))
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
			horizontalVelocity -= 
				(sideVelocity == SVec3::Zero( ) ? sideVelocity : SVec3::Normalize( sideVelocity )) * 
				cappedSideDecel;

			{
				// cache the forwardVelocity length for the calculations ahead
				auto forwardVelocityLen = forwardVelocity.Length( );
				auto forwardVelocityLenInv = forwardVelocityLen == 0.0f ? 0.0f : 1.0f / forwardVelocityLen;

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
					auto lenInv = len == 0.0f ? 0.0f : 1.0f / len;

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

		void SweptControllerSystem::sweptCollision(SweptController& controller, SVec3 sweptVelocity, float timeLeft, bool kinematic)
		{
			auto trans = controller.m_transform;

			// Used to keep track of consecutive contacted surface normals
			// to detect unsolvable configurations that require special handling.
			std::vector<SVec3> normals;

			// Sentinel value, serves no purpose other than removing a conditional statement.
			normals.push_back(SVec3( 0.0f ));

			// The number of iterations used is arbitrary.
			// Some geometrical configurations can take as much as 10-20 iterations to resolve.
			// Almost always resolves within a few iterations otherwise.
			// 20 iterations was found to behave well through lots of testing.
			for (int iterCount = 0; iterCount < 20; ++iterCount)
			{
				// Used to denote when a collision that can be resolved was found in the sweep.
				auto collision = false;

				// ContinuousCollider computes the time of impact for every object encountered in
				// the collider's trajectory with the given velocity and timestep, sorted by first time of impact.
				auto rigidbody = controller.m_rigidbody;
				physics::SweepOutput output;

				m_physics->Sweep( rigidbody.Get( ), sweptVelocity, timeLeft, /*TODO: Filter*/
							      output, physics::SweepType::SWEEP_ALL_HITS, true );

				for (size_t i = 0, n = output.hit.size( ); i < n; ++i)
				{
					// Normal of the contacted surface.
					auto normal = output.normal[ i ];

					// Get the velocity relative to the direction of the contacted surface.
					auto relVel = -SVec3::Dot( normal, sweptVelocity );

					// Check for separating velocity.
					// Considering near zero relative velocities will waste iterations on numerical error
					// and lock up possible movement for the controller.
					if (relVel < controller.m_epsilon)
						continue;

					// Sending a collision for other game logic since this controller does not
					// advance the character into contact with the detected object.
					// this.SendCollisionEvent( Events.SweptCollision, result );

					// Check collision groups for skip resolution after detection event is sent
					//if (this.SkipResolution(this.Collider, result.OtherCollider))
					//    continue;

					// Keep track of kinematic objects for next update.
					controller.addIfKinematic( m_world->GetEntityUnique( output.entity[ i ] ) );
					
					// Move forward to the first time of impact.
					// A time of 0 is valid, it just wont result in any translation.

					// Account for the case when the distance returned in the sweep is less than the amount
					// we're taking off to account for Bullet's epsilon internally
					const float resolutionEpsilon = 0.01f;
					auto delta = sweptVelocity * output.time[ i ];
					auto normVel = SVec3::Normalize( sweptVelocity );

					timeLeft -= output.time[ i ];

					trans->SetWorldPosition( 
						trans->GetWorldPosition( ) + delta - 
						normVel * resolutionEpsilon 
					);

					// Determine what kind of surface was contacdted
					auto ground = isGroundSurface( controller, normal );
					auto ceiling = isCeilingSurface( controller, normal );

					// Moving along the ground.
					// This case is for maintaining the controller's horizontal speed
					// while moving over sloped ground surfaces.
					// If that behavior is not desired, then add '&& kinematic'
					// to the condition because this is still needed for kinematic sweeps.
					if (controller.m_grounded && ground)
					{
						sweptVelocity = skewProjection( controller, sweptVelocity, controller.m_worldUp, normal );
					}
					// Moving into a wall while grounded.
					else if (controller.m_grounded && !ground && !ceiling)
					{
						// Kinematic sweep can have vertical velocity when grounded.
						// Have to project along the wall and maintain verticle speed.
						auto verticalSweep = SVec3::ProjectToNorm( sweptVelocity, controller.m_worldUp );
						sweptVelocity -= verticalSweep;

						if (kinematic)
						{
							verticalSweep = skewProjection( 
								controller, verticalSweep, 
								normal - SVec3::ProjectToNorm( normal, controller.m_worldUp ), normal
							);
						}
						else
						{
							verticalSweep = SVec3::Zero( );
						}

						// Project out the horizontal motion that's in the direction of the surface.
						auto horizontalNormal = normal - SVec3::ProjectToNorm( normal, controller.m_worldUp );
						horizontalNormal.Normalize( );
						sweptVelocity -= SVec3::ProjectToNorm( sweptVelocity, horizontalNormal );
						sweptVelocity += verticalSweep;
					}
					// Jumping upward into the ceiling.
					else if (!controller.m_grounded && ceiling && sweptVelocity.Dot( controller.m_worldUp ) > 0.0f)
					{
						// Remove vertical velocity for sweep.
						sweptVelocity -= SVec3::ProjectToNorm( sweptVelocity, controller.m_worldUp );

						// Remove vertical velocity for controller
						// so that upward motion does not persist.
						if (!kinematic && controller.m_controllerVelocity.Dot( controller.m_worldUp ) > 0.0f)
						{
							controller.m_controllerVelocity -= SVec3::ProjectToNorm( controller.m_controllerVelocity, controller.m_worldUp );
						}
					}
					// Falling onto the ground.
					// If moving up a slope fast enough and then jumping, contacting the ground can cancel a jump,
					// the check for not jumping is to prevent that and can be removed if desired.
					else if (!kinematic && !controller.m_grounded && !controller.m_jumping && ground)
					{
						// Remove vertical velocity only on first impact with ground,
						// the controller does not have vertical velocity when grounded.
						controller.m_controllerVelocity -= SVec3::ProjectToNorm( controller.m_controllerVelocity, controller.m_worldUp );

						// Continue sweep using the controller's horizontal velocity
						// so that landing on a slope while moving does not cause a large change in velicity.
						sweptVelocity = controller.m_controllerVelocity;

						// Need to set grounded as soon as it happens so that the following
						// iterations behave with the correct conditions,
						// and so that this case is not repeated in the same update.
						controller.m_grounded = true;
					}
					// All non specific behavior cases
					else
					{
						// Project out all velocity that's in the direction of the contact surface.
						sweptVelocity -= SVec3::ProjectToNorm( sweptVelocity, normal );
					}

					// When contacting a wall, do not want any velocity into the wall to persist between updates.
					if (!kinematic && !ground && !ceiling)
					{
						// The horizontal component of the resulting sweepVelocity will have been
						// projected out of the contacted wall surface, this can be used for
						// determining if the controllerVelocity should persist in that direction.
						// The persisting vertical component is taken from the ControllerVelocity
						// so that it behaves the same on the ground and in the air.
						auto horizontalSweep = sweptVelocity - SVec3::ProjectToNorm( sweptVelocity, controller.m_worldUp );
						auto verticalVelocity = SVec3::ProjectToNorm( controller.m_controllerVelocity, controller.m_worldUp );
						auto horizontalVelocity = controller.m_controllerVelocity - verticalVelocity;

						// Don't want to take the sweep velocity if it's not in the direction of the controller,
						// otherwise falling down a sloped wall onto the ground will cause it to
						// slide backwards instead of stopping on the ground.
						if (horizontalSweep.Dot( horizontalVelocity ) > 0.0f || horizontalSweep.Length( ) < controller.m_epsilon)
						{
							controller.m_controllerVelocity = horizontalSweep + verticalVelocity;
						}
					}

					// Add surface normal for checking edge cases
					normals.push_back( normal );

					// Get normals from the last two consecutively contacted surfaces.
					// When contacting the very first surface, the initial zero vector
					// that was added will cause the condition to intentionally fail.
					auto normal1 = normals[ normals.size( ) - 2 ];
					auto normal2 = normals[ normals.size( ) - 1 ];

					// Check for acute angle between surfaces.
					// If angle is acute (less than 90 degrees), then sweep will project back and forth
					// between surfaces forever without any progress.
					// Problem must be resolved on the axis created by both surfaces.
					if (normal1.Dot( normal2 ) < -controller.m_epsilon)
					{
						// Get axis of plane intersection.
						// Must be normalized to maintain correct velocity magnitudes.
						auto slopeAxis = SVec3::Cross( normal1, normal2 );
						slopeAxis = SVec3::Normalize( slopeAxis );

						// Kinematic sweep should move along the axis in all cases.
						// Controller sweep, if not grounded, could get in other
						// unsolvable configurations for the character's motion.
						if (kinematic || controller.m_grounded)
						{
							sweptVelocity = SVec3::ProjectToNorm( sweptVelocity, slopeAxis );
						}
						// Character is stuck sliding between two walls.
						else
						{
							// Because the character is not grounded, if AirAcceleration is zero
							// then the character will be unable to move, or fall, when the slope
							// is perpendicular to the WorldUp axis.
							controller.m_controllerVelocity = SVec3::ProjectToNorm( controller.m_controllerVelocity, slopeAxis );
							sweptVelocity = controller.m_controllerVelocity;

							// If slope is not perpendicular, setting AirTraction to zero
							// will force the character to slide down the slope.
							if (abs( slopeAxis.Dot( controller.m_worldUp ) ) > controller.m_epsilon)
							{
								controller.m_airTraction = 0.0f;
							}
						}
					}

					// Only resolve the first non-seperating contact.
					collision = true;
					break;
				}

				// If no intersection to resolve from the sweep.
				if (!collision)
				{
					// Move by the remainng sweep amount.
					trans->SetWorldPosition( trans->GetWorldPosition( ) + sweptVelocity * timeLeft );

					// No more interactions to do, sweep is completed.
					break;
				}
			}
		}

		void SweptControllerSystem::sweptCompleted(SweptController& controller)
		{
			// if (controller.m_forwardEvents)
			{
				// this.EventTracker.SendEvents("Collision");
			}

			// this.Owner.DispatchEvent(Events.SweptCompleted, ZilchEvent());

			updateKinematicList( controller );
		}

		void SweptControllerSystem::snapToGround(SweptController &controller)
		{
			if (controller.m_grounded)
			{
				// Assume not grounded anymore and reset flag only if ground is still detected below the character.
				controller.m_grounded = false;

				// Maximum distance allowed to snap in opposite direction of worldUp.
				auto maxDisplacement = controller.m_worldUp * -controller.m_groundSnapDistance;

				// Passing a timestep of 1 makes a displacement no different than velocity.
				// Time of impacts will return a value between 0 and 1,
				// effectively parameterizing the allowed snap distance.
				auto rigidbody = controller.m_rigidbody;
				physics::SweepOutput output;

				m_physics->Sweep( rigidbody.Get( ), maxDisplacement, 1.0f, /*TODO: Filter*/
							      output, physics::SweepType::SWEEP_ALL_HITS, true );

				for (size_t i = 0, n = output.hit.size( ); i < n; ++i)
				{
					auto normal = output.normal[ i ];
					auto relativeVel = -SVec3::Dot( normal, maxDisplacement );

					// Ignore seperating velocity for the same reasons as the regular sweep.
					if (relativeVel < controller.m_epsilon)
						continue;

					// Skip everything that's not ground.
					// Doesn't matter if something else is hit first because the controller
					// shouldn't unground when on the edge of the ground and a wall slope simultaniously.
					// The allowed distance from the ground is meant to be fairly small anyway.
					if (!isGroundSurface( controller, normal ))
						continue;

					// Sending this event unique from the sweep events so the user can
					// choose to do something only when in contact with the ground.
					// TODO: this.SendCollisionEvent( Events.GroundSnapCollision, result );

					// Check collision groups for skip resolution after detection event is sent.
					// TODO: if (controller.skipResolution( this.collider, result.othercollider ))
					// continue;

					// Keep track of kinematic objects for next update.
					controller.addIfKinematic( m_world->GetEntityUnique( output.entity[ i ] ) );

					auto trans = controller.m_transform;
					
					trans->SetWorldPosition( trans->GetWorldPosition( ) + maxDisplacement * output.time[ i ] );

					// Reset flag since ground was detected.
					controller.m_grounded = true;

					// First detection with a ground surface is all that's needed.
					break;
				}
			}
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
