#include "Precompiled.h"

#include "AIState.h"
namespace ursine
{
    namespace sm
    {

        void AIState::OnEnter(StateMachine *machine)
        {
            Enter(reinterpret_cast<AIStateMachine *>(machine));
        }

        void AIState::OnUpdate(StateMachine *machine)
        {
            Update(reinterpret_cast<AIStateMachine *>(machine));
        }

        void AIState::OnExit(StateMachine *machine)
        {
            Exit(reinterpret_cast<AIStateMachine *>(machine));
        }

    }
}