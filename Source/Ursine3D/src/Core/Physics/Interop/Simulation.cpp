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