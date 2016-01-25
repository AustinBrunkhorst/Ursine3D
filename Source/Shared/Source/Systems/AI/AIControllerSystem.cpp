#include "Precompiled.h"
#include "AIControllerSystem.h"

namespace ursine
{
    namespace ecs
    {
        ENTITY_SYSTEM_DEFINITION(AIControllerSystem);

        AIControllerSystem::AIControllerSystem(World* world) 
            : EntitySystem(world)
        {
        }

        void AIControllerSystem::OnInitialize()
        {
            m_world->Listener(this)
                .On(WORLD_ENTITY_COMPONENT_ADDED,   &AIControllerSystem::onComponentAdded)
                .On(WORLD_ENTITY_COMPONENT_REMOVED, &AIControllerSystem::onComponentRemoved)
                .On(WORLD_UPDATE, &AIControllerSystem::onUpdate);
        }

        void AIControllerSystem::OnRemove()
        {
            m_world->Listener(this)
                .Off(WORLD_ENTITY_COMPONENT_ADDED, &AIControllerSystem::onComponentAdded)
                .Off(WORLD_ENTITY_COMPONENT_REMOVED, &AIControllerSystem::onComponentRemoved)
                .Off(WORLD_UPDATE, &AIControllerSystem::onUpdate);
        }

        void AIControllerSystem::onComponentAdded(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, ComponentEventArgs);

            if (args->component->Is<AIMovementController>())
            {
                m_kinematicControllers.push_back(static_cast<AIMovementController *>(args->component));
            }
        }

        void AIControllerSystem::onComponentRemoved(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, ComponentEventArgs);

            if (args->component->Is<AIMovementController>())
            {
                m_kinematicControllers.erase(std::find(
                    m_kinematicControllers.begin(),
                    m_kinematicControllers.end(),
                    args->component));
            }
        }

        void AIControllerSystem::onUpdate(EVENT_HANDLER(World))
        {
            for (auto KiController : m_kinematicControllers)
            {
                KiController->Update();
            }
        }
    }
}
