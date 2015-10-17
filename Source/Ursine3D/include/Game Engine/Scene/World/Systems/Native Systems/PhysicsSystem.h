// Physics System
// - World System
// - Add / Remove collision shapes and bodies to / from the simulation
// - initializes a new instance of a simulation whenever it is constructed
// - debug / performance debug info for this world's physics sim

// AGGREGATE THE SIMULATION

#include "EntitySystem.h"
#include "PhysicsInterop.h"

namespace ursine
{
    namespace ecs
    {
        class PhysicsSystem
        {
            ENTITY_SYSTEM;
        public:
            
        private:
            physics::Simulation simulation;
        };
    }
}
