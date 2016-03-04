/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** VineLookForInRangePlayersState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "VineAIState.h"

class VineLookForInRangePlayersState : public VineAIState
{
public:
    VineLookForInRangePlayersState(void);

    void Enter(VineAIStateMachine *machine) override;
    void Update(VineAIStateMachine *machine) override;

private:
    bool m_inRange;
    bool m_inView;
    ursine::SVec3 m_originalForward;
};
