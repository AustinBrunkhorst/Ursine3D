/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** VineDeathState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "VineAIState.h"

class EntityAnimator;

class VineDeathState : public VineAIState
{
public:
    VineDeathState(void);

    void Enter(VineAIStateMachine *machine) override;

private:

    ursine::ecs::Entity *owner;

    void onAnimationFinished(EVENT_HANDLER(EntityAnimator));

};
