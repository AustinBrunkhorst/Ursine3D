/* ---------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TankSlamState.cpp
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include <iostream>

#include "Application.h"

#include "AIStatemachine.h"
#include "TankSlamState.h"
#include <EntityAnimation/EntityAnimatorComponent.h>



namespace ursine
{
    namespace sm
    {

        const std::string TankSlamState::m_downAnimation = "TankDown";
        const std::string TankSlamState::m_upAnimation = "TankUp";

        TankSlamState::TankSlamState(const std::string &name, float damage, float downTime)
            : AIState(name)
            , m_downTime(downTime)
            , m_timer(0.0f)
            , m_damage(damage)
            , m_finished(false)
            , m_animating(false)
            , m_animator(nullptr)
            , m_state(SlamState::Down)
        {
        }

        void TankSlamState::Enter(AIStateMachine* machine)
        {
            m_animator = machine->GetEntity()->GetComponentInChildren<EntityAnimator>();

            m_timer = m_downTime;

            m_state = SlamState::Down;

            m_finished = false;

            playAnimation(m_downAnimation);

            //m_animator->Listener(this)
            //    .On(EntityAnimatorEvent::FinishedAnimating, &TankSlamState::onAnimationFinished);
        }

        void TankSlamState::Update(AIStateMachine* machine)
        {
            // if we are animating, there is nothing to do
            if (m_animating || m_state == SlamState::Down)
                return;

            m_timer -= Application::Instance->GetDeltaTime();

            if (m_timer < 0)
            {
                playAnimation(m_upAnimation);
            }
        }

        void TankSlamState::Exit(AIStateMachine* machine)
        {
        }

        void TankSlamState::playAnimation(const std::string &clip)
        {
            m_animator->Play(clip);

            m_animating = true;

            m_animator->Listener(this)
                .On(EntityAnimatorEvent::FinishedAnimating, &TankSlamState::onAnimationFinished);
        }

        void TankSlamState::onAnimationFinished(EVENT_HANDLER(EntityAnimator))
        {
            EVENT_ATTRS(EntityAnimator, ursine::ecs::EntityEventArgs);

            switch (m_state)
            {
            case SlamState::Down:
            {
                m_state = SlamState::Up;

                //TODO: Spawn particles!!!!!!!!!

                break;
            }
            case SlamState::Up:
            {
                m_finished = true;
                break;
            }
            }
            

            m_animating = false;

            sender->Listener(this)
                .Off(EntityAnimatorEvent::FinishedAnimating, &TankSlamState::onAnimationFinished);
        }

    }
}