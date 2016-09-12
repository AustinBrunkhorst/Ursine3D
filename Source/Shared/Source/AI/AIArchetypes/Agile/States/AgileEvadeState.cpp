/* ---------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** AgileEvadeState.cpp
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
#include "AgileEvadeState.h"
#include <EntityAnimation/EntityAnimatorComponent.h>



namespace ursine
{
    namespace sm
    {
        AgileEvadeState::AgileEvadeState(const std::string &name)
            : AIState(name)
        {
        }

        void AgileEvadeState::Enter(AIStateMachine* machine)
        {
        }

        void AgileEvadeState::Update(AIStateMachine* machine)
        {
        }

        void AgileEvadeState::Exit(AIStateMachine* machine)
        {
        }
    }
}