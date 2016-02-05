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
            
            //typedef AIHorde::EnemyType EnemyType;

        public:
            AIReasonerSystem(World* world);


            void OnInitialize(void) override;
            void OnRemove(void) override;

        private:
            void onComponentAdded(EVENT_HANDLER(World));
            void onComponentRemoved(EVENT_HANDLER(World));

            void onUpdate(EVENT_HANDLER(World));

            Meta(Enable)
            std::vector< std::vector<Component::Handle<AIHorde> > > m_reasoners;

        }Meta( Enable, AutoAddEntitySystem );
    }
}
