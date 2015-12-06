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
        if (animator->GetAnimation( ) != "Player_Winning")
        {
            animator->SetAnimationTimePosition(0);
            animator->SetAnimation("Player_Winning");
            animator->SetLooping(true);
        }
    }
    else if(m_state == Dead)
    {
        if (animator->GetAnimation( ) != "Player_Dead")
        {
            animator->SetAnimationTimePosition(0);
            animator->SetAnimation("Player_Dead");
            animator->SetLooping(false);
        }
    }
    else if(m_state == Jumping)
    {
        if (animator->GetAnimation( ) != "Player_Jump")
        {
            animator->SetAnimationTimePosition(0);
            animator->SetAnimation("Player_Jump");
            animator->SetLooping(false);

            if(!m_shooting)
            {
                
            }
        }
    }
    else if(m_state == Running)
    {
        if (animator->GetAnimation( ) != "Player_Run")
        {
            animator->SetAnimationTimePosition(0);
            animator->SetAnimation("Player_Run");
            animator->SetLooping(true);

            if (!m_shooting)
            {

            }
        }
    }
    else if(m_state == Idle)
    {
        if (animator->GetAnimation( ) != "Player_Idle")
        {
            animator->SetAnimationTimePosition(0);
            animator->SetAnimation("Player_Idle");
            animator->SetLooping(true);

            if (!m_shooting)
            {

            }
        }
    }
}

void PlayerAnimation::SetPlayerState(const PlayerState state)
{
    m_state = state;
}

void PlayerAnimation::SetShooting(const bool isShooting)
{
    m_shooting = isShooting;
}