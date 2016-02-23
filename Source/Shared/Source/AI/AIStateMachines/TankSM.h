#include "AIStateMachine.h"
#include "AIState.h"

namespace ursine
{
    namespace sm
    {
        class TankStateMachine : public AIStateMachine
        {
        public:
            TankStateMachine(ecs::Entity *aiActor);

        private:



        };

        class TankWalkState : public AIState
        {
        public:
            virtual void Enter(AIStateMachine *);
            virtual void Update(AIStateMachine *);
            virtual void Exit(AIStateMachine *);
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