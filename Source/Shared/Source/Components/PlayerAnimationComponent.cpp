/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerAnimationComponent.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "PlayerAnimationComponent.h"
#include <Game Engine/Scene/Component/Native Components/AnimatorComponent.h>

NATIVE_COMPONENT_DEFINITION(PlayerAnimation);

PlayerAnimation::PlayerAnimation( )
    : BaseComponent( )
    , m_shooting(false)
    , m_state(Idle) {}

void PlayerAnimation::Update()
{
    auto *animator = GetOwner( )->GetComponent<ursine::ecs::Animator>( );

    if(m_state == Winning)
    {
        if (animator->GetCurrentState( ) != "Win")
        {
            animator->SetAnimationTimePosition(0);
            animator->SetCurrentState("Win");
            animator->SetLooping(true);
        }
    }
    else if(m_state == Dead)
    {
        if (animator->GetCurrentState( ) != "Die")
        {
            animator->SetAnimationTimePosition(0);
            animator->SetCurrentState("Die");
            animator->SetLooping(false);
        }
    }
    else if(m_state == Jumping)
    {
        if (animator->GetCurrentState( ) != "Jump")
        {
            animator->SetAnimationTimePosition(0);
            animator->SetCurrentState("Jump");
            animator->SetLooping(false);

            if(!m_shooting)
            {
                
            }
        }
    }
    else if(m_state == Running)
    {
        if (animator->GetCurrentState( ) != "Run")
        {
            animator->SetAnimationTimePosition(0);
            animator->SetCurrentState("Run");
            animator->SetLooping(true);

            if (!m_shooting)
            {

            }
        }
    }
    else if(m_state == Idle)
    {
        if (animator->GetCurrentState( ) != "Idle")
        {
            animator->SetAnimationTimePosition(0);
            animator->SetCurrentState("Idle");
            animator->SetLooping(true);

            if (!m_shooting)
            {

            }
        }
    }
}

void PlayerAnimation::SetPlayerState(const PlayerState state)
{
    // A hack to make sure the death animation is persistant and set once.
    if (m_state == Dead)
        return;

    m_state = state;
}

void PlayerAnimation::SetShooting(const bool isShooting)
{
    m_shooting = isShooting;
}

void PlayerAnimation::UnsetDead(void)
{
    m_state = Idle;
}