/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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
#include "Sweeping.h"
#include "CollisionEventArgs.h"

namespace ursine
{
    // forward declaration
    namespace ecs
    {
        class Entity;
		class Rigidbody;
		class Body;
    }

    namespace physics
    {
        // forward declaration
        class Body;
        class Rigidbody;
        class Ghost;

        class Simulation
        {
        public:
            Simulation(const SVec3 &gravity = SVec3( 0.0f, -10.0f, 0.0f ));
            ~Simulation(void);

            // Step the simulation forward
            // If the maxSubSteps > 0, it will interpolate motion between fixedTimeSteps
            void Step(float timeStep, int maxSubSteps = 7, float fixedTimeStep = 1.0f / 60.0f);
            void DebugDrawSimulation(void);

            void SetDebugDrawer(DebugDrawer *debugDrawer);
            
            // Add a rigid body to the simulation
            void AddRigidbody(Rigidbody *rigidbody);
            void RemoveRigidbody(Rigidbody *rigidbody);

            // Add a body to the simulation (not rigid or soft)
            void AddBody(Body *body);
            void RemoveBody(Body *body);

            // Add a ghost to the simulation
            void AddGhost(Ghost *ghost);
            void RemoveGhost(Ghost *ghost);

            bool Raycast(const RaycastInput &input, RaycastOutput &output, RaycastType type);

			bool Sweep(ColliderBase *collider, BodyBase *body, const SVec3 &velocity, 
					   float dt, SweepOutput &output, SweepType type, bool sorted = false);

            void SetGravity(const SVec3 &gravity);
            SVec3 GetGravity(void) const;

            void ClearContacts(Rigidbody &rigidbody);

			void DispatchCollisionEvents(void);

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
            btDbvtBroadphase m_overlappingPairCache;

            // the default constraint solver. For parallel processing 
            // you can use a different solver (see Extras/BulletMultiThreaded)
            btSequentialImpulseConstraintSolver m_solver;

            // This is needed for supporting ghost objects in the simulation
            btGhostPairCallback m_ghostCallback;

            btSoftRigidDynamicsWorld *m_dynamicsWorld;
		#endif

			bool contactCallbackEnabled(const BodyBase *body);

			void dispatchContactEvent(
                const BodyBase *thisBody, 
                const BodyBase *otherBody,
				const ecs::EntityHandle &thisEntity, 
                const ecs::EntityHandle &otherEntity, 
                PersistentManifold *manifold
            );

			const ecs::EntityHandle &getEntityHandle(const BodyBase *body);

			void calculateContactRelativeVelocity(
			    const BodyBase *thisBody, 
                const BodyBase *otherBody, 
                Contact *contact
			);
        };
    }
}
