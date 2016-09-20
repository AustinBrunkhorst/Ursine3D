/* ---------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** NukerShootState.cpp
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include <iostream>

#include "Application.h"

#include "AIStatemachine.h"
#include "NukerShootState.h"
#include <EntityAnimation/EntityAnimatorComponent.h>



namespace ursine
{
    namespace sm
    {
        NukerShootState::NukerShootState(const std::string &name)
            : AIState(name)
            , m_finished(false)
        {
        }

        void NukerShootState::Enter(AIStateMachine* machine)
        {
        }

        void NukerShootState::Update(AIStateMachine* machine)
        {
        }

        void NukerShootState::Exit(AIStateMachine* machine)
        {
        }
    }
}