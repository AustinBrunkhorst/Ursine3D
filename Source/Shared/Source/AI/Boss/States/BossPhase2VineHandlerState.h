/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossPhase2VineHandlerState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include "BossAIState.h"

class BossPhase2VineHandlerState : public BossAIState
{
public:
    BossPhase2VineHandlerState(void);

    void Update(BossAIStateMachine *machine) override;

};
