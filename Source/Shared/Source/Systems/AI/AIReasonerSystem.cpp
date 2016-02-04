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

                auto enemy = static_cast<Component::Handle<AIHorde>>(static_cast<AIHorde *>(args->component));

                EnemyType eType = enemy->GetEnemyType();

                if (eType < EnemyType::INVALID_ENEMY)
                {
                    m_reasoners[enemy->GetEnemyType()].push_back(enemy);
                }
            }
        }

        void AIReasonerSystem::onComponentRemoved(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, ComponentEventArgs);

            if (args->component->Is<AIHorde>())
            {
                auto component = static_cast<Component::Handle<AIHorde>>(static_cast<AIHorde *>(args->component));

                auto eType = component->GetEnemyType();

                // Remove it from our list
                m_reasoners[eType].erase(std::find(
                    m_reasoners[eType].begin(),
                    m_reasoners[eType].end(),
                    component
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

            Vec3 targetPos = target->GetTransform()->GetWorldPosition();

            for (int i = EnemyType::Hordeling; i < EnemyType::INVALID_ENEMY; ++i)
            for (auto hordeComp : m_reasoners[i])
            {
                hordeComp->SetTarget(targetPos);
            }

            for (auto boomlings : m_reasoners[EnemyType::Boomling])
            {
                // check distance from target
                auto m_transform = boomlings->GetOwner()->GetTransform();
                if (targetPos.Distance( m_transform->GetWorldPosition( ) ) < 8.0f)
                {
                    //TODO: Add effects
                    boomlings->GetOwner()->Delete();
                }
            }
        }
    }
}
