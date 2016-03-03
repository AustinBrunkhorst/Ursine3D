#pragma once

#include "AIStateMachine.h"
#include "AIState.h"

namespace ursine
{
    namespace sm
    {
        class FodderAI;

        class FodderAIStateMachine : public AIStateMachine
        {
        public:
            FodderAIStateMachine();

            virtual void Initialize(ecs::Entity *actor) override;

            ~FodderAIStateMachine();

            //void Update(void) override;

            FodderAI *GetAI(void) const;

        private:
            void onCollide(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED));

        };

    }
}