#include "UrsinePrecompiled.h"

#include "Simulation.h"

#include "Body.h"
#include "Rigidbody.h"
#include "SphereCollider.h"

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
    }
}