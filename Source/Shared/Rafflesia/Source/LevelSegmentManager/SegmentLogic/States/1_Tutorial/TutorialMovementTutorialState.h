/* ---------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TutorialMovementTutorialState.h
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include "SegmentLogicState.h"

#include "EntityAnimatorComponent.h"

class  TutorialMovementTutorialState : public SegmentLogicState
{
public:

   TutorialMovementTutorialState(void);

   void Enter(SegmentLogicStateMachine *machine) override;

   bool CanExit(void) override { return m_complete; }

private:

   bool m_complete;

   void onCameraAnimFinish(EVENT_HANDLER(EntityAnimator));

};
