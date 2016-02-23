#include "StateMachine.h"
#include "Entity.h"

namespace ursine
{
    namespace sm
    {


        class AIState;

        class AIStateMachine : public StateMachine
        {
        public:
            AIStateMachine(ecs::Entity *aiActor);

            ecs::Entity *GetEntity(void) const;
        private:
            ecs::Entity *m_actorEntity;
        };
    }
}