#include "UrsinePrecompiled.h"

#include "KinematicChildSystem.h"

namespace ursine
{
    namespace ecs
    {
        KinematicChildSystem::KinematicChildSystem(World *world)
            : FilterSystem(world, Filter().All<KinematicChild, RigidBody2D>(),
                           SIMULATION_PRIORITY - 10) // make sure this happens before Simulation integration
        {

        }

        KinematicChildSystem::~KinematicChildSystem(void)
        {

        }

        void KinematicChildSystem::Begin(void)
        {
            _dt = gApplication->GetDelta();
        }

        void KinematicChildSystem::Process(Entity *entity)
        {
            auto trans = entity->GetTransform();
            auto body = entity->GetComponent<RigidBody2D>();
            auto kin_child = entity->GetComponent<KinematicChild>();

            // I've found uses for this system when an object doesn't have a parent.
            // Therefore we don't need this at the moment.
            // UAssert(trans->Parent() != nullptr, "A Kinematic Child must have a parent.")

            // update and store the last and current position
            auto last_pos = kin_child->_last_position;
            auto curr_pos = kin_child->_last_position = trans->PositionWC();

            // find the delta
            auto delta = curr_pos - last_pos;

            // differentiate to find this frame's approximate velocity
            // V = delta_pos / delta_time
            body->SetVelocity(Vec2(delta / _dt));
        }
    }
}
