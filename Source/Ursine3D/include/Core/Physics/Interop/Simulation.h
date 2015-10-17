// Simulation
// Interop class
// - Contains the physics engine's equivalent

#pragma once

#include "PhysicsInteropConfig.h"

namespace ursine
{
    namespace physics
    {
        class Simulation
        {
        public:
            Simulation(const SVec3 &gravity = SVec3( 0.0f, -10.0f, 0.0f ));

        private:
#ifdef BULLET_PHYSICS
            // collision configuration contains default setup for memory,
            // collision setup. Advanced users can create their own configuration.
            btDefaultCollisionConfiguration m_collisionConfig;

            // use the default collision dispatcher. For parallel processing 
            // you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
            btCollisionDispatcher *m_dispatcher;

            // btDbvtBroadphase is a good general purpose broadphase. 
            // You can also try out btAxis3Sweep.
            btBroadphaseInterface *m_overlappingPairCache;

            // the default constraint solver. For parallel processing 
            // you can use a different solver (see Extras/BulletMultiThreaded)
            btSequentialImpulseConstraintSolver *m_solver;

            btSoftRigidDynamicsWorld *m_dynamicsWorld;
#endif
        };
    }
}
