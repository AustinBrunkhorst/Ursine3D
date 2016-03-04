/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BossAIState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <State.h>

class BossAIStateMachine;

class BossAIState : public ursine::sm::State
{
public:

    BossAIState(const std::string &name)
        : State( name ) { }

    void OnEnter(ursine::sm::StateMachine *machine) override final;
    void OnUpdate(ursine::sm::StateMachine *machine) override final;
    void OnExit(ursine::sm::StateMachine *machine) override final;

    virtual void Enter(BossAIStateMachine *machine) { };
    virtual void Update(BossAIStateMachine *machine) { };
    virtual void Exit(BossAIStateMachine *machine) { };
};
