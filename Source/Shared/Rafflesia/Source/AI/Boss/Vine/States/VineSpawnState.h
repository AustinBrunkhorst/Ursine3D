/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** VineSpawnState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "VineAIState.h"

class VineSpawnState : public VineAIState
{
public:
    VineSpawnState(void);

    bool CanExit(void) override { return m_finished; }

    void Enter(VineAIStateMachine *machine) override;

private:
    bool m_finished;

    void onAnimationFinished(EVENT_HANDLER(ursine::ecs::Entity));
};
