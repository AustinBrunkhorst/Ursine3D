/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimatorControllerState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <State.h>

class AnimatorControllerStateMachine;

class AnimatorControllerState : public ursine::sm::State
{
public:

    AnimatorControllerState(const std::string &name)
        : State( name ) { }

    void OnEnter(ursine::sm::StateMachine *machine) override final;
    void OnUpdate(ursine::sm::StateMachine *machine) override final;
    void OnExit(ursine::sm::StateMachine *machine) override final;

    virtual void Enter(AnimatorControllerStateMachine *machine) { };
    virtual void Update(AnimatorControllerStateMachine *machine) { };
    virtual void Exit(AnimatorControllerStateMachine *machine) { };
};
