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
#include "VineAIComponent.h"

class VineDeathState : public VineAIState
{
public:
    VineDeathState(void);

    void Enter(VineAIStateMachine *machine) override;

private:

    ursine::ecs::EntityHandle owner;

    void onAnimationFinished(EVENT_HANDLER(ursine::ecs::Entity));

};
