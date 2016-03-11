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
        BomberExplodeState::BomberExplodeState(const std::string &name, WalkState *walkState, float damage)
            : AIState(name)
            , m_walk(walkState)
            , m_finished(false)
        {
        }

        void BomberExplodeState::Enter(AIStateMachine* machine)
        {
            // start that dope explode animation

            m_walk->Enter(machine);
        }

        void BomberExplodeState::Update(AIStateMachine* machine)
        {
            // we gon start moving at the player, maybe speed up over time
            m_walk->Update(machine);
        }

        void BomberExplodeState::Exit(AIStateMachine* machine)
        {
            m_walk->Exit(machine);

            // if we are here, the animation ended so it is time to explode.
            
            // AOE damage

            // dope particles

            // destroy ourselves
        }

    }
}