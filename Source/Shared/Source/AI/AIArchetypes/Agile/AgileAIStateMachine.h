/* ---------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TankAIStateMachine.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "AIStateMachine.h"
#include "AIState.h"

namespace ursine
{
    namespace sm
    {
        class AgileAI;

        class AgileAIStateMachine : public AIStateMachine
        {
        public:
            AgileAIStateMachine();

            //~TankAIStateMachine();

            //void Update(void) override;

        private:
            //void onCollide(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED));

        };
    }
}