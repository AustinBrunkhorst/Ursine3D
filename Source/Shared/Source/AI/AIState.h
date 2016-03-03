#pragma once

#include "State.h"

class StateMachine;
class AIStateMachine;

namespace ursine
{
    namespace sm
    {

        class AIState : public ursine::sm::State
        {
        public:
            AIState(std::string &name) : State(name) {}


            virtual void OnEnter(StateMachine *machine) override final;
            virtual void OnUpdate(StateMachine *machine) override final;
            virtual void OnExit(StateMachine *machine) override final;

            virtual void Enter(AIStateMachine *) = 0;
            virtual void Update(AIStateMachine *) = 0;
            virtual void Exit(AIStateMachine *) = 0;
        };

    }
}