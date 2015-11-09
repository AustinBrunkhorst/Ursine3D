/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Simulation.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "PhysicsInteropConfig.h"
#include "DebugDrawer.h"
#include "Raycasting.h"

namespace ursine
{
    // forward declaration
    namespace ecs
    {
        class Entity;
    }

    namespace physics
    {
        // forward declaration
        class Body;
        class Rigidbody;

        class Simulation
        {
        public:
            Simulation(const SVec3 &gravity = SVec3( 0.0f, 0.0f, 0.0f ));
            ~Simulation(void);

            // Step the simulation forward
            // If the maxSubSteps > 0, it will interpolate motion between fixedTimeSteps
            void Step(float timeStep, int maxSubSteps = 1, float fixedTimeStep = 1.0f / 60.0f);

            void SetDebugDrawer(DebugDrawer *debugDrawer);
            
            // Add a rigid body to the simulation
            void AddRigidbody(Rigidbody *rigidbody);
            void RemoveRigidbody(Rigidbody *rigidbody);

            // Add a body to the simulation (not rigid or soft)
            void AddBody(Body *body);
            void RemoveBody(Body *body);

            bool Raycast(const RaycastInput &input, RaycastOutput &output, RaycastType type);

        private:

            // terminate the simulation
            void destroySimulation(void);

#ifdef BULLET_PHYSICS
            // collision configuration contains default setup for memory,
            // collision setup. Advanced users can create their own configuration.
            btDefaultCollisionConfiguration m_collisionConfig;

            // use the default collision dispatcher. For parallel processing 
            // you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
            btCollisionDispatcher *m_dispatcher;

            // btDbvtBroadphase is a good general purpose broadphase. 
            // You can also try out btAxis3Sweep.
            btDbvtBroadphase *m_overlappingPairCache;

            // the default constraint solver. For parallel processing 
            // you can use a different solver (see Extras/BulletMultiThreaded)
            btSequentialImpulseConstraintSolver *m_solver;

            btSoftRigidDynamicsWorld *m_dynamicsWorld;
#endif
        };
    }
}
