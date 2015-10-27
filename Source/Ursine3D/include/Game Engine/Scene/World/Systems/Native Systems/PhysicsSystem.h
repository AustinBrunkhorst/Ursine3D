// Physics System
// - World System
// - Add / Remove collision shapes and bodies to / from the simulation
// - initializes a new instance of a simulation whenever it is constructed
// - debug / performance debug info for this world's physics sim

// AGGREGATE THE SIMULATION

#include "FilterSystem.h"
#include "Simulation.h"

namespace ursine
{
    namespace ecs
    {
        class PhysicsSystem 
            : public FilterSystem
        {
            ENTITY_SYSTEM;

        public:
            PhysicsSystem(World *world);
            ~PhysicsSystem(void);

        protected:
            void Process(Entity *entity) override;

        private:
            physics::Simulation m_simulation;

            void OnInitialize(void) override;
            void OnRemove(void) override;

            void onComponentAdded(EVENT_HANDLER(World));
            void onComponentRemoved(EVENT_HANDLER(World));

            void onUpdate(EVENT_HANDLER(World));

            Filter m_collisionShapes;

        } Meta(Enable);
    }
}
