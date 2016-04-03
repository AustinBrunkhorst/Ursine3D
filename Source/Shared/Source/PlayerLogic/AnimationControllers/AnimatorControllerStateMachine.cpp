/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SegmentLogicStateMachine.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "AnimatorControllerStateMachine.h"

AnimatorControllerStateMachine::AnimatorControllerStateMachine(const std::string &name, ursine::ecs::Animator *animator)
    : m_animator( animator )
    , m_name( name )
{
}

ursine::ecs::Animator *AnimatorControllerStateMachine::GetAnimator(void)
{
    return m_animator;
}
