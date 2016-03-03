#pragma once

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
            AIStateMachine(void);

            virtual void Initialize(ecs::Entity *actor);

            ecs::Entity *GetEntity(void) const;

        protected:
            //TODO: replace with the things austin is using now
            ecs::Entity *m_actorEntity;
        };
    }
}