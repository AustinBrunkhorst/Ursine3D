/* ---------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TankSlamState.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "AIState.h"
#include "Entity.h"
#include "Vec3.h"


class AIStateMachine;

class EntityAnimator;

namespace ursine
{
    namespace sm
    {
        class TankSlamState : public AIState
        {
        public:
            TankSlamState(const std::string &name, float damage, float downTime);

            bool CanExit(void) override { return m_finished; }

            void Enter(AIStateMachine *machine) override;
            void Update(AIStateMachine *machine) override;
            void Exit(AIStateMachine *machine) override;

        private:

            static const std::string m_downAnimation;
            static const std::string m_upAnimation;

            float m_downTime;

            float m_timer;

            float m_damage;

            bool m_finished;
            bool m_animating;

            enum class SlamState
            {
                Down,
                Up
            };

            EntityAnimator *m_animator;

            SlamState m_state;

            void playAnimation(const std::string &clip);

            void onAnimationFinished(EVENT_HANDLER(EntityAnimator));
        };

    }
}