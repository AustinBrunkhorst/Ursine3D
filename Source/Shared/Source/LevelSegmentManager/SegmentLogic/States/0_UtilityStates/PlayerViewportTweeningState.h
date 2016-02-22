/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerViewportTweeningState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "SegmentLogicState.h"

enum class ViewportTweenType
{
    SplitOutUpDown,
    SplitInUpDown,
    
    SplitOutRightLeft,
    SplitInRightLeft,

    SplitOutLeftRight,
    SplitInLeftRight,

    OutUp,
    OutDown,
    InUp,
    InDown,

    OutRight,
    OutLeft,
    InRight,
    InLeft
};

class PlayerViewportTweeningState : public SegmentLogicState
{
public:
    PlayerViewportTweeningState(ViewportTweenType type);

    void Enter(SegmentLogicStateMachine *machine) override;

private:
    ViewportTweenType m_type;

};