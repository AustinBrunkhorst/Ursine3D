/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** CombatBowl1IntroCinematic.h
**
** Author:
** - JordanEllis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include "SegmentLogicState.h"

#include "CameraAnimatorComponent.h"

class CombatBowl1IntroCinematicState : public SegmentLogicState
{
public:

    CombatBowl1IntroCinematicState(void);

    void Enter(SegmentLogicStateMachine *machine) override;

    bool CanExit(void) override { return m_complete; }

private:

    bool m_complete;

    void onCameraAnimFinish(EVENT_HANDLER(CameraAnimator));

};
