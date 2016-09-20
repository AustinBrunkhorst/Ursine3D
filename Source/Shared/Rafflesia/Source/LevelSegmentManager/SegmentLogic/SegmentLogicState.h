/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SegmentLogicState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <State.h>

class SegmentLogicStateMachine;

class SegmentLogicState : public ursine::sm::State
{
public:

    SegmentLogicState(const std::string &name)
        : State( name ) { }

    void OnEnter(ursine::sm::StateMachine *machine) override final;
    void OnUpdate(ursine::sm::StateMachine *machine) override final;
    void OnExit(ursine::sm::StateMachine *machine) override final;

    virtual void Enter(SegmentLogicStateMachine *machine) { };
    virtual void Update(SegmentLogicStateMachine *machine) { };
    virtual void Exit(SegmentLogicStateMachine *machine) { };
};
