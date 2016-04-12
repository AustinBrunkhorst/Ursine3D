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

namespace ViewportTweenType_enum
{
    enum ViewportTweenType
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
        InLeft,

        SqueezeInHorizontal,
        SqueezeOutHorizontal,

        SqueezeInVertical,
        SqueezeOutVertical,

        SqueezeIn,
        SqueezeOut,

        NUM
    };
}

typedef ViewportTweenType_enum::ViewportTweenType ViewportTweenType;

class PlayerViewportTweeningState : public SegmentLogicState
{
public:
    PlayerViewportTweeningState(ViewportTweenType type, bool blocking, bool camerasActive);

    bool CanExit(void) override { return !m_blocking; }

    void Enter(SegmentLogicStateMachine *machine) override;

    void Exit(SegmentLogicStateMachine *machine) override;

private:
    ViewportTweenType m_type;

    bool m_blocking;
    bool m_camerasActive;

};