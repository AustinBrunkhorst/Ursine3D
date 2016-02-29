/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BossVineState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <State.h>

class BossVineStateMachine;

class BossVineState : public ursine::sm::State
{
public:

    BossVineState(const std::string &name)
        : State( name ) { }

    void OnEnter(ursine::sm::StateMachine *machine) override final;
    void OnUpdate(ursine::sm::StateMachine *machine) override final;
    void OnExit(ursine::sm::StateMachine *machine) override final;

    virtual void Enter(BossVineStateMachine *machine) { };
    virtual void Update(BossVineStateMachine *machine) { };
    virtual void Exit(BossVineStateMachine *machine) { };
};
