#include "UrsinePrecompiled.h"

#include "Simulation.h"

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
    }
}