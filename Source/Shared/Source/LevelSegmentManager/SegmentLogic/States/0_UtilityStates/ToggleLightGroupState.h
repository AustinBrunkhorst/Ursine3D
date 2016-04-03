/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ToggleLightGroupState.h
**
** Author:
** - JordanEllis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include "SegmentLogicState.h"

class ToggleLightGroupState : public SegmentLogicState
{
public:

    ToggleLightGroupState(bool toggle, std::vector<std::string> groupEntities);

    void Enter(SegmentLogicStateMachine *machine) override;

private:

    std::vector<std::string> m_groupEntities;

    bool m_toggle;
};
