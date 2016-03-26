/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossInvulnerableToggleState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include "BossAIState.h"

class BossInvulnerableToggleState : public BossAIState
{
public:
    BossInvulnerableToggleState(bool toggle);

    void Enter(BossAIStateMachine *machine) override;

private:
    bool m_toggle;
};
