/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SimulationSystem.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////
#ifndef SIMULATION_SYSTEM_H
#define SIMULATION_SYSTEM_H
////////////////////////////////////////////////////////////////////////////////

#include "EntitySystem.h"

#include <unordered_map>

namespace Ursine
{
    // Forward declarations
    class Joint;
    class DistanceJoint;
    class Manifold;
    class ManifoldKey;

    namespace ECS
    {
        class Simulation
            : public EntitySystem
        {
            friend class PhysicsDebugDrawer;

            // Look for bodies being added or removed from the simulation
            void onComponentAdded(EVENT_HANDLER(EntityManager));
            void onComponentRemoved(EVENT_HANDLER(EntityManager));

            // Update the simulation
            void onUpdate(EVENT_HANDLER(SystemManager));

            // Step (1) Collision detection
            void generateManifolds(void);

            // Step (2) Integrate forces
            void integrateForces(void);

            // Step (3) Pre-step
            void preStep(void);

            // Step (4) Apply impulse
            void applyImpulse(void);

            // Step (5) Integrate velocities
            void integrateVelocities(void);

            // Step (6) Post-step
            void postStep(void);

            // All bodies added to the simulation
            std::vector<RigidBody2D*> _bodies;

            // All distance joints added to the simulation
            std::vector<Joint *> _joints;

            // map of all of the manifolds
            std::map<ManifoldKey, Manifold> _manifolds;

            float _dt;

            PhysicsDebugDrawer *_debug_drawer;

            ////////////////////////////////////////////////////////////////
            // This is used for registering pairs of entities that will   //
            // ignore each other in the simulation.                       //
            ////////////////////////////////////////////////////////////////
            // Rules to this data structure:                              //
            // (1) Key is the lesser of the two IDs in the ignored pair.  //
            // (2) Value is a vector of IDs that are paired with the key. //
            ////////////////////////////////////////////////////////////////
            typedef std::vector<EntityUniqueID> IDVec;
            typedef std::unordered_map<EntityUniqueID, IDVec> IDMap;
            IDMap _ignore_map;

            // Helper for getting two IDs in ascending order
            void getIDs(RigidBody2D *body_0, RigidBody2D *body_1,
                        EntityUniqueID &id_0, EntityUniqueID &id_1);

        public:
            Simulation(World *world);
            ~Simulation(void);

            void OnInitialize(void) override;
            void OnRemove(void) override;

            DistanceJoint *AddDistanceJoint(RigidBody2D *body_0, RigidBody2D *body_1,
                                 float distance, bool max_distance_only = false);

            // Remove the joint connecting the two bodies
            void RemoveJoint(RigidBody2D *body_0, RigidBody2D *body_1);

            // Remove all joints that involve this object
            void RemoveJoints(RigidBody2D *body);

            // Raycast through the simulation
            bool RayCast(RayCastOutput &output, const RayCastInput &input,
                         const LayerMask &mask = LayerMask());

            // Raycast through the simulation, with added predicate functionality, 
            // allowing user to throw out specific bodies on a different criteria
            // Predicate Function Format:
            // bool function(RigidBody2D *body)
            // { return false if we DO NOT want to use this body; }
            template <typename PredicateFN>
            bool RayCast(RayCastOutput &output, const RayCastInput &input,
                         const LayerMask &mask, PredicateFN predicate);


            // Queries a region (AABB), returning true if there are intersections,
            // and filling a vector of intersecting entities
            bool Simulation::RegionQuery(const AABB &region, std::vector<Entity *> &entities);

            // Register an ignored pair of bodies
            void SetIgnoreCollision(RigidBody2D *body_0, RigidBody2D *body_1, bool flag = true);

            // Check to see if two bodies are being ignored
            bool GetIgnoreCollision(RigidBody2D *body_0, RigidBody2D *body_1);
        };
    }
}

#include "SimulationSystem.hpp"

////////////////////////////////////////////////////////////////////////////////
#endif // !SIMULATION_SYSTEM_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_SYSTEMS

#ifndef SIMULATION_SYSTEM_REGISTERED
#define SIMULATION_SYSTEM_REGISTERED

namespace Ursine
{
    namespace ECS
    {
        RegisterEntitySystem(Simulation);
    }
}

#endif

#endif