/* ---------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** FodderAIStateMachine.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** -------------------------------------------------------------------------*/
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

            virtual void Initialize(ecs::EntityHandle actor) override;

            ~FodderAIStateMachine();

            //void Update(void) override;

        private:
            void onCollide(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED));

        };

    }
}