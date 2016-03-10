// this file is used to store any AIstates that can be shared across actors
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
        class DamageState : public AIState
        {
        public:
            DamageState(std::string name, float damage);

            virtual void Enter(AIStateMachine *stateMachine) override;
            virtual void Update(AIStateMachine *stateMachine) override;
            virtual void Exit(AIStateMachine *stateMachine) override;

            virtual bool CanExit(void) override { return m_finished; }

            void SetAnimationClip(const std::string &clip);

        private:
            void applyDamage(ursine::ecs::Entity* obj, const ursine::SVec3& contact, float damage);

            void onAnimationFinished(EVENT_HANDLER(EntityAnimator));

            std::string m_clipName;

            EntityAnimator *m_animator;

            float m_damage;

            bool m_finished;
        };
    }
}
