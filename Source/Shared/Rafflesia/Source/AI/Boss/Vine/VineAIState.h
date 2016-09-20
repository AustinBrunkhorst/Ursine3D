/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** VineAIState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <State.h>

class VineAIStateMachine;

class VineAIState : public ursine::sm::State
{
public:

    VineAIState(const std::string &name)
        : State( name ) { }

    void OnEnter(ursine::sm::StateMachine *machine) override final;
    void OnUpdate(ursine::sm::StateMachine *machine) override final;
    void OnExit(ursine::sm::StateMachine *machine) override final;

    virtual void Enter(VineAIStateMachine *machine) { };
    virtual void Update(VineAIStateMachine *machine) { };
    virtual void Exit(VineAIStateMachine *machine) { };
};
