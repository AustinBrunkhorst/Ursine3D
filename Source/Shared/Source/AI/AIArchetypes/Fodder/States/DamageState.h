// this file is used to store any AIstates that can be shared across actors
#pragma once

#include "AIState.h"
#include "Entity.h"
#include "Vec3.h"

class AIStateMachine;

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

        private:
            void applyDamage(ursine::ecs::Entity* obj, const ursine::SVec3& contact, float damage);

            void onCollide(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED));

            float m_damage;
        };
    }
}
