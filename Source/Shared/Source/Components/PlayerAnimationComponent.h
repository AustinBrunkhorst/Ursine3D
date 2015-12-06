/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerAnimationComponent.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

class PlayerAnimation
    : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    enum PlayerState
    {
        Winning = 0,
        Dead,
        Jumping,
        Running,
        Idle,

        StateCount
    };

public:
    PlayerAnimation(void);

    Meta(Disable)
    void Update(void);

    Meta(Disable)
    void SetPlayerState(const PlayerState state);

    Meta(Disable)
    void SetShooting(const bool isShooting);

    void UnsetDead(void);

private:
    bool m_shooting;
    PlayerState m_state;
}Meta(Enable, DisplayName("PlayerAnimation"));
