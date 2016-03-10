/* ---------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BomberExplodeState.cpp
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
#include "BomberExplodeState.h"
#include <EntityAnimation/EntityAnimatorComponent.h>



namespace ursine
{
    namespace sm
    {
        BomberExplodeState::BomberExplodeState(const std::string &name, float damage, float downTime)
            : AIState(name)
            , m_finished(false)
        {
        }

        void BomberExplodeState::Enter(AIStateMachine* machine)
        {

        }

        void BomberExplodeState::Update(AIStateMachine* machine)
        {

        }

        void BomberExplodeState::Exit(AIStateMachine* machine)
        {
        }

    }
}