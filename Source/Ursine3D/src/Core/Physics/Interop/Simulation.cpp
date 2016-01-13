/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Simulation.cpp
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Simulation.h"

#include "Body.h"
#include "BodyComponent.h"

#include "Rigidbody.h"
#include "RigidbodyComponent.h"

#include "EntityEvent.h"

namespace ursine
{
    namespace physics
    {
        Simulation::Simulation(const SVec3 &gravity)
        {
        #ifdef BULLET_PHYSICS

            m_dispatcher = new btCollisionDispatcher( &m_collisionConfig );
            m_overlappingPairCache = new btDbvtBroadphase( );
            m_solver = new btSequentialImpulseConstraintSolver( );

            m_dynamicsWorld = new btSoftRigidDynamicsWorld(
                m_dispatcher,
                m_overlappingPairCache,
                m_solver,
                &m_collisionConfig
            );

            m_dynamicsWorld->setGravity({ 
                gravity.X( ), gravity.Y( ), gravity.Z( ) 
            });

        #endif
        }

        Simulation::~Simulation(void)
        {
            destroySimulation( );
        }

        void Simulation::Step(float timeStep, int maxSubSteps, float fixedTimeStep)
        {
        #ifdef BULLET_PHYSICS

            m_dynamicsWorld->stepSimulation( timeStep, maxSubSteps, fixedTimeStep );

        #endif
        }

        void Simulation::DebugDrawSimulation(void)
        {
        #ifdef BULLET_PHYSICS

            m_dynamicsWorld->debugDrawWorld( );

        #endif
        }

        void Simulation::SetDebugDrawer(DebugDrawer *debugDrawer)
        {
        #ifdef BULLET_PHYSICS

            m_dynamicsWorld->setDebugDrawer( debugDrawer );

        #endif
        }

        void Simulation::AddRigidbody(Rigidbody *body)
        {
        #ifdef BULLET_PHYSICS

            m_dynamicsWorld->addRigidBody( body );

        #endif
        }

        void Simulation::RemoveRigidbody(Rigidbody *body)
        {
        #ifdef BULLET_PHYSICS

            m_dynamicsWorld->removeRigidBody( body );

        #endif
        }

        void Simulation::AddBody(Body *body)
        {
        #ifdef BULLET_PHYSICS

            m_dynamicsWorld->addCollisionObject( body );

        #endif
        }

        void Simulation::RemoveBody(Body* body)
        {
        #ifdef BULLET_PHYSICS

            m_dynamicsWorld->removeCollisionObject( body );

        #endif
        }

        void Simulation::SetGravity(const SVec3& gravity)
        {
        #ifdef BULLET_PHYSICS

            btVector3 grav( gravity.X( ), gravity.Y( ), gravity.Z( ) );

            m_dynamicsWorld->setGravity( grav );

        #endif
        }

        SVec3 Simulation::GetGravity(void) const
        {
        #ifdef BULLET_PHYSICS

            auto grav = m_dynamicsWorld->getGravity( );

            return { grav.getX( ), grav.getY( ), grav.getZ( ) };

        #endif
        }

        bool Simulation::Raycast(const RaycastInput& input, RaycastOutput& output, RaycastType type)
        {
            output.entity.clear( );
            output.hit.clear( );
            output.normal.clear( );

        #ifdef BULLET_PHYSICS

            auto &startP = input.start;
            auto &endP = input.end;

            btVector3 start( startP.X( ), startP.Y( ), startP.Z( ) );
            btVector3 end( endP.X( ), endP.Y( ), endP.Z( ) );

            if (type == RAYCAST_CLOSEST_HIT)
            {
                btDynamicsWorld::ClosestRayResultCallback closestHit( start, end );

                m_dynamicsWorld->rayTest( start, end, closestHit );

                if (closestHit.hasHit( ))
                {
                    auto &hitP = closestHit.m_hitPointWorld;
                    auto &normP = closestHit.m_hitNormalWorld;

                    output.hit.emplace_back( 
                        hitP.getX( ), 
                        hitP.getY( ), 
                        hitP.getZ( ) 
                    );

                    output.normal.emplace_back( 
                        normP.getX( ), 
                        normP.getY( ), 
                        normP.getZ( ) 
                    );

                    output.entity.push_back( 
                        closestHit.m_collisionObject->getUserIndex( ) 
                    );

                    return true;
                }
                else
                    return false;
            }
            else // RAYCAST_ALL_HITS
            {
                btDynamicsWorld::AllHitsRayResultCallback allHit( start, end );

                m_dynamicsWorld->rayTest( start, end, allHit );

                if (allHit.hasHit( ))
                {
                    auto &hitP = allHit.m_hitPointWorld;
                    auto &normP = allHit.m_hitNormalWorld;

                    for (int i = 0, n = hitP.size( ); i < n; ++i)
                    {
                        output.hit.emplace_back( 
                            hitP[ i ].getX( ), 
                            hitP[ i ].getY( ), 
                            hitP[ i ].getZ( ) 
                        );

                        output.normal.emplace_back( 
                            normP[ i ].getX( ), 
                            normP[ i ].getY( ), 
                            normP[ i ].getZ( ) 
                        );

                        output.entity.push_back( 
                            allHit.m_collisionObjects[ i ]->getUserIndex( ) 
                        );
                    }

                    return true;
                }
                else
                    return false;
            }

        #endif
        }

	    void Simulation::ClearContacts(Rigidbody& rigidbody)
        {
        #ifdef BULLET_PHYSICS

            auto proxy = rigidbody.getBroadphaseProxy( );

            m_dynamicsWorld->getPairCache( )
                ->removeOverlappingPairsContainingProxy( proxy, m_dispatcher );

        #endif
        }

	    void Simulation::DispatchCollisionEvents(void)
	    {
			CollisionEventArgs args;

		#ifdef BULLET_PHYSICS

			auto *dispatcher = m_dynamicsWorld->getDispatcher( );
			int numManifolds = dispatcher->getNumManifolds( );

			for (int i = 0; i < numManifolds; ++i)
			{
				auto *manifold = dispatcher->getManifoldByIndexInternal( i );

				auto *objA = static_cast<const BodyBase*>( manifold->getBody0( ) );
				auto *objB = static_cast<const BodyBase*>( manifold->getBody1( ) );

				// Get each body's entity pointer
				ecs::Entity *entityA = getEntityPointer( objA ), 
					        *entityB = getEntityPointer( objB );
				
				bool sendA = contactCallbackEnabled( objA ), 
					 sendB = contactCallbackEnabled( objB );

				// Process the event for Entity A
				if (sendA)
				{
					dispatchContactEvent( objA, objB, entityA, entityB, manifold );
				}

				// Process the event for Entity B
				if (sendB)
				{
					dispatchContactEvent( objB, objA, entityB, entityA, manifold );
				}
			}

		#endif
	    }

	    void Simulation::destroySimulation(void)
        {
        #ifdef BULLET_PHYSICS

            //delete dynamics world
            delete m_dynamicsWorld;

            //delete solver
            delete m_solver;

            //delete broadphase
            delete m_overlappingPairCache;

            //delete dispatcher
            delete m_dispatcher;

        #endif
        }

		bool Simulation::contactCallbackEnabled(const BodyBase *body)
        {
	        // Determine whether the entities have contact callbacks enabled
			if (body->getInternalType( ) == BT_BODY)
				return static_cast<ecs::Body*>( body->getUserPointer( ) )->GetEnableContactCallback( );
			
			if (body->getInternalType( ) == BT_RIGID_BODY)
				return static_cast<ecs::Rigidbody*>( body->getUserPointer( ) )->GetEnableContactCallback( );
			
			return false;
        }

	    void Simulation::dispatchContactEvent(const BodyBase* thisBody, const BodyBase* otherBody, 
											  ecs::Entity* thisEntity, ecs::Entity* otherEntity, PersistentManifold *manifold)
	    {
			CollisionEventArgs args;

			args.thisEntity = thisEntity;
			args.otherEntity = otherEntity;

		#ifdef BULLET_PHYSICS

			auto numContacts = manifold->getNumContacts( );
			
			// To determine if a manifold bullet has is just starting,
			// or persisting, we look at the lifetime (frames) of each contact
			int greatestLifetime = 0;

			for (int i = 0; i < numContacts; ++i)
			{
				args.contacts.emplace_back( );
				auto &contact = args.contacts.back( );

				btManifoldPoint &pt = manifold->getContactPoint( i );

				contact.normal = SVec3( pt.m_normalWorldOnB );
				contact.point = SVec3( pt.getPositionWorldOnB( ) );
				contact.appliedImpulse = pt.getAppliedImpulse( );
				contact.penDistance = pt.getDistance( );
				calculateContactRelativeVelocity( thisBody, otherBody, &contact );

				greatestLifetime = math::Max( greatestLifetime, pt.getLifeTime( ) );
			}

			// If the collision has just started
			if (greatestLifetime < 2)
				thisEntity->Dispatch( ecs::ENTITY_COLLISION_STARTED, &args );
			else
				thisEntity->Dispatch( ecs::ENTITY_COLLISION_PERSISTED, &args );

		#endif
	    }

	    ecs::Entity *Simulation::getEntityPointer(const BodyBase *body)
        {
	        return static_cast<ecs::Component*>( body->getUserPointer( ) )->GetOwner( );
        }

		 void Simulation::calculateContactRelativeVelocity(const BodyBase *thisBody, 
														  const BodyBase *otherBody, 
														  Contact *contact)
	    {
		#ifdef BULLET_PHYSICS

			// Calculate the relative velocity
			contact->relativeVelocity = SVec3( thisBody->getInterpolationLinearVelocity( ) );

			SVec3 otherVelocity;

			auto linVel = otherBody->getInterpolationLinearVelocity( );
			auto angVel = otherBody->getInterpolationAngularVelocity( );
			auto center = otherBody->getWorldTransform( ).getOrigin( );
			auto point = contact->point.ToBullet( );

			otherVelocity = SVec3( angVel.cross( point - center ) + linVel );

			contact->relativeVelocity = contact->relativeVelocity - otherVelocity;

		#endif
	    }
    }
}