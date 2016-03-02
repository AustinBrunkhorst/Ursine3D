#include "AIStateMachine.h"
#include "AIState.h"

namespace ursine
{
    namespace sm
    {
        class TankAIStateMachine : public AIStateMachine
        {
        public:
            TankAIStateMachine(ecs::Entity *aiActor);

        private:



        };

        class TankThwompState : public AIState
        {
        public:
            virtual void Enter(AIStateMachine *);
            virtual void Update(AIStateMachine *);
            virtual void Exit(AIStateMachine *);
        };

    }
}