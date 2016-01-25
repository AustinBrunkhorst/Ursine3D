#include "Precompiled.h"

#include "AIReasonerSystem.h"

namespace ursine
{
    namespace ecs
    {
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
            /*
            EVENT_ATTRS(World, ComponentEventArgs);

            if (args->component->Is<AIReasoner>() || args->component->)
            {
                m_reasoners.push_back(static_cast<AIReasoner *>(args->component));
            }*/
        }

        void AIReasonerSystem::onComponentRemoved(EVENT_HANDLER(World))
        {
        }

        void AIReasonerSystem::onUpdate(EVENT_HANDLER(World))
        {
        }
    }
}