/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ToggleCameraActiveState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "SegmentLogicState.h"

class ToggleCameraActiveState : public SegmentLogicState
{
public:

    ToggleCameraActiveState(const std::string &entityName, bool toggle);

    void Enter(SegmentLogicStateMachine *machine) override;

private:
    std::string m_entityName;
    bool m_toggle;
};
