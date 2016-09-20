/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ScreenFadeState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "SegmentLogicState.h"

#include <Color.h>

class ScreenFadeState : public SegmentLogicState
{
public:

    ScreenFadeState(
        float inTime, float pauseTime, 
        float outTime, const ursine::Color &color
    );

    void Enter(SegmentLogicStateMachine *machine) override;

private:

    float m_inTime, m_pauseTime, m_outTime;
    ursine::Color m_color;

};
