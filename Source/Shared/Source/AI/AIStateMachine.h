#pragma once

#include "StateMachine.h"
#include "Entity.h"

class AIState;

namespace ursine
{
    namespace sm
    {
        class AIStateMachine : public StateMachine
        {
        public:
            AIStateMachine(void);

            virtual void Initialize(ecs::EntityHandle actor);

            ecs::EntityHandle GetEntity(void) const;

        protected:
            ecs::EntityHandle m_actorEntity;
        };
    }
}