// this file is used to store any AIstates that can be shared across actors

#include "AIState.h"
#include "Entity.h"
#include "Vec3.h"

class AIStateMachine;

namespace ursine
{
    namespace sm
    {
        class PauseState : public AIState
        {
        public:
            PauseState(std::string name, float delay);

            virtual void Enter(AIStateMachine *stateMachine) override;
            virtual void Update(AIStateMachine *stateMachine) override;
            virtual void Exit(AIStateMachine *stateMachine) override;

            virtual bool CanExit(void) override;

        private:

            float m_delay;
            float m_timer;
        };
    }
}
