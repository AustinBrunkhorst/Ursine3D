/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** InitializeSegmentState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "SegmentLogicState.h"

class LockPlayerCharacterControllerState : public SegmentLogicState
{
public:

    LockPlayerCharacterControllerState(bool lockP1Move, bool lockP1Look,
                                       bool lockP2Move, bool lockP2Look);

    void Enter(SegmentLogicStateMachine *machine) override;

private:

    bool m_lockP1Move : 1;
    bool m_lockP1Look : 1;
    bool m_lockP2Move : 1;
    bool m_lockP2Look : 1;
};
