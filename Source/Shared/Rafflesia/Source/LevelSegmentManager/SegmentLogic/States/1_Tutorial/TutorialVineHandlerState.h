/* ---------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TutorialVineHandlerState.h
**
** Author:
** - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include "SegmentLogicState.h"

#include "HealthComponent.h"

class TutorialVineHandlerState : public SegmentLogicState
{
public:

    TutorialVineHandlerState(ursine::resources::ResourceReference vineArchetype);

    void Enter(SegmentLogicStateMachine *machine) override;

    bool CanExit(void) override { return false; }

private:

    int m_vineCount;
    ursine::resources::ResourceReference m_vineArchetype;
    SegmentLogicStateMachine *m_machine;

    void onVineDeath(EVENT_HANDLER(Health));
};
