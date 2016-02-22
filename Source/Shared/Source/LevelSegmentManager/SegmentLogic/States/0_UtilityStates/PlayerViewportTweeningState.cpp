/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerViewportTweeningState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "PlayerViewportTweeningState.h"

#include "SegmentLogicStateIncludes.h"

using namespace ursine;
using namespace ecs;

PlayerViewportTweeningState::PlayerViewportTweeningState(ViewportTweenType type)
    : SegmentLogicState( "Player Viewport Tween" )
{
    m_type = type;
}

void PlayerViewportTweeningState::Enter(SegmentLogicStateMachine *machine)
{
    switch (m_type)
    {
        case ViewportTweenType::SplitOutUpDown:
        {
            break;
        }
        case ViewportTweenType::SplitInUpDown:
        {
            break;
        }
        case ViewportTweenType::SplitOutRightLeft:
        {
            break;
        }
        case ViewportTweenType::SplitInRightLeft:
        {
            break;
        }
        case ViewportTweenType::SplitOutLeftRight:
        {
            break;
        }
        case ViewportTweenType::SplitInLeftRight:
        {
            break;
        }
        case ViewportTweenType::OutUp:
        {
            break;
        }
        case ViewportTweenType::OutDown:
        {
            break;
        }
        case ViewportTweenType::InUp:
        {
            break;
        }
        case ViewportTweenType::InDown:
        {
            break;
        }
        case ViewportTweenType::OutRight:
        {
            break;
        }
        case ViewportTweenType::OutLeft:
        {
            break;
        }
        case ViewportTweenType::InRight:
        {
            break;
        }
        case ViewportTweenType::InLeft:
        {
            break;
        }
    };
}
