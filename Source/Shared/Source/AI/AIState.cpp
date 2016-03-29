#include "Precompiled.h"

#include "AIState.h"
#include <PlayerLogic/PlayerIdComponent.h>
#include "WaypointComponent.h"

namespace ursine
{
    namespace sm
    {

        void AIState::OnEnter(StateMachine *machine)
        {
            Enter(reinterpret_cast<AIStateMachine *>(machine));
        }

        void AIState::OnUpdate(StateMachine *machine)
        {
            Update(reinterpret_cast<AIStateMachine *>(machine));
        }

        void AIState::OnExit(StateMachine *machine)
        {
            Exit(reinterpret_cast<AIStateMachine *>(machine));
        }

        Vec3 AIState::GetTargetPlayerPosition(ecs::World *world)
        {
            // try to grab the player as the target
            auto players = world->GetEntitiesFromFilter( ecs::Filter().All< PlayerID >());

            ecs::EntityHandle target = nullptr;

            // if we can't find a player, just go to the closest waypoint
            if (players.size() < 1)
            {
                // if we can't find either, don't bother updating the targets
                auto selected = world->GetEntitiesFromFilter( ecs::Filter().All<ecs::Waypoint >());

                if (selected.size() < 1)
                {
                    return Vec3::Zero();
                }

                target = selected[0];
            }
            else
            {
                target = players[0];
            }

            return target->GetTransform()->GetWorldPosition();
        }

    }
}
