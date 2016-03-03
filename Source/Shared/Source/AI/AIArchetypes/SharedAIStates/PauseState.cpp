#include "Precompiled.h"

#include <iostream>

#include "PauseState.h"
#include <Utilities/Application/Application.h>

using namespace ursine::ecs;
namespace ursine
{
    namespace sm
    {
        PauseState::PauseState(std::string name, float delay)
            : AIState( name )
            , m_delay( delay )
            , m_timer( delay )
        {
        }


        void PauseState::Enter(AIStateMachine *stateMachine)
        {
            m_timer = m_delay;
        }
        void PauseState::Update(AIStateMachine *stateMachine)
        {
            m_timer -= Application::Instance->GetDeltaTime();
        }
        void PauseState::Exit(AIStateMachine *stateMachine)
        {
            // pretty sure we can actually just do nothing on exit
        }

        bool PauseState::CanExit()
        {
            return m_timer <= 0.0f;
        }
    }
}
