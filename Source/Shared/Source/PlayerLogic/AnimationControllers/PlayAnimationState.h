/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayAnimationState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "AnimatorControllerState.h"

class PlayAnimationState : public AnimatorControllerState
{
public:
    PlayAnimationState(
        const std::string &stateName, 
        bool blocking = false, 
        float playback = 1.0f
    );

    bool CanExit(void) override { return m_finished; }

    void Enter(AnimatorControllerStateMachine *machine) override;
    void Exit(AnimatorControllerStateMachine *machine) override;

private:
    std::string m_stateName;
    bool m_blocking;
    float m_playback;
    bool m_finished;

    void onAnimationFinish(EVENT_HANDLER(ursine::ecs::Entity));
};
