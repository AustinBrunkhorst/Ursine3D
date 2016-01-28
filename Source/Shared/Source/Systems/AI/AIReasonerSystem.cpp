#include "Precompiled.h"

#include "AIReasonerSystem.h"
#include <Components/PlayerIdComponent.h>
#include <Components/AI/WaypointComponent.h>

namespace ursine
{
    namespace ecs
    {
        ENTITY_SYSTEM_DEFINITION(AIReasonerSystem);

        AIReasonerSystem::AIReasonerSystem(World* world): EntitySystem(world)
        {
        }

        void AIReasonerSystem::OnInitialize()
        {
            m_world->Listener(this)
                .On(WORLD_ENTITY_COMPONENT_ADDED, &AIReasonerSystem::onComponentAdded)
                .On(WORLD_ENTITY_COMPONENT_REMOVED, &AIReasonerSystem::onComponentRemoved)
                .On(WORLD_UPDATE, &AIReasonerSystem::onUpdate);
        }

        void AIReasonerSystem::OnRemove()
        {
            m_world->Listener(this)
                .Off(WORLD_ENTITY_COMPONENT_ADDED, &AIReasonerSystem::onComponentAdded)
                .Off(WORLD_ENTITY_COMPONENT_REMOVED, &AIReasonerSystem::onComponentRemoved)
                .Off(WORLD_UPDATE, &AIReasonerSystem::onUpdate);
        }

        void AIReasonerSystem::onComponentAdded(EVENT_HANDLER(World))
        {
            
            EVENT_ATTRS(World, ComponentEventArgs);

            if (args->component->Is<AIHorde>())
            {
                m_reasoners.push_back(static_cast<AIHorde *>(args->component));
            }
        }

        void AIReasonerSystem::onComponentRemoved(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, ComponentEventArgs);

            if (args->component->Is<AIHorde>())
            {
                // Remove it from our list
                m_reasoners.erase(std::find(
                    m_reasoners.begin(),
                    m_reasoners.end(),
                    args->component
                    ));
            }
        }

        void AIReasonerSystem::onUpdate(EVENT_HANDLER(World))
        {
            //NOTE: Later, a lot of these checks could be done by a sensing layer

            // try to grab the player as the target
            auto players = m_world->GetEntitiesFromFilter( Filter().All< PlayerID >() );

            Entity *target = nullptr;

            // if we can't find a player, just go to the closest waypoint
            if (players.size() < 1)
            {
                // if we can't find either, don't bother updating the targets
                auto selected = m_world->GetEntitiesFromFilter(Filter().All< Waypoint >());

                if (selected.size() < 1)
                {
                    return;
                }

                target = selected[0];
            }
            else
            {
                target = players[0];
            }

            Vec3 TargetPos = target->GetTransform()->GetWorldPosition();

            for (auto hordeComp : m_reasoners)
            {
                hordeComp->SetTarget(TargetPos);
            }
        }
    }
}
