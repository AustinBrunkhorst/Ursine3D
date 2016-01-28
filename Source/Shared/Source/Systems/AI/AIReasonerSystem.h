#include "EntitySystem.h"
#include "AIHordeReasoner.h"

namespace ursine
{
    namespace ecs
    {
        // we need to have an easy to access list of componentIDs for the reasoners.
        // probably add the file in the AI component file. Will be weird since that should be in shared
        // actually we can probably just add an AI folder to shared components. Will research how to do that in the morning

        class AIReasonerSystem : EntitySystem
        {
            ENTITY_SYSTEM;

            AIReasonerSystem(World* world);


            void OnInitialize(void) override;
            void OnRemove(void) override;

            void onComponentAdded(EVENT_HANDLER(World));
            void onComponentRemoved(EVENT_HANDLER(World));

            void onUpdate(EVENT_HANDLER(World));

            std::vector<AIHorde *> m_reasoners;

        }Meta( Enable );
    }
}
