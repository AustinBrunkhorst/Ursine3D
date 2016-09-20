/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GunReloadFinishedState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "AnimatorControllerState.h"

class GunReloadFinishedState : public AnimatorControllerState
{
public:
    GunReloadFinishedState(void);

    void Enter(AnimatorControllerStateMachine *machine);

};
