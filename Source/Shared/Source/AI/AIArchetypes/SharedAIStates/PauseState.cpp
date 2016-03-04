#include "Precompiled.h"

#include <iostream>
#include "AIStateMachine.h"

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
        {
        }


        void PauseState::Enter(AIStateMachine *stateMachine)
        {
            auto why = stateMachine->GetFloat("PauseTimer");
            stateMachine->SetFloat("PauseTimer", m_delay);

        }
        void PauseState::Update(AIStateMachine *stateMachine)
        {

            auto currTime = stateMachine->GetFloat( "PauseTimer" );

            stateMachine->SetFloat( "PauseTimer", currTime - Application::Instance->GetDeltaTime() );
        }
        void PauseState::Exit(AIStateMachine *stateMachine)
        {
            // pretty sure we can actually just do nothing on exit
            stateMachine->SetBool("HitPlayer", false);

        }
    }
}
