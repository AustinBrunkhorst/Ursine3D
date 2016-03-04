#include "Precompiled.h"

#include <iostream>

#include <HealthSystem/HealthComponent.h>
#include "CollisionEventArgs.h"
#include "EntityEvent.h"

#include "AIStatemachine.h"
#include "DamageState.h"
#include <AI/AIArchetypes/Fodder/FodderAIComponent.h>
#include <PlayerLogic/PlayerIDComponent.h>
#include <EntityAnimation/EntityAnimatorComponent.h>
#include <DamageSystem/DamageOnCollideComponent.h>


using namespace ursine::ecs;
namespace ursine
{
    namespace sm
    {
        DamageState::DamageState(std::string name, float damage)
            : AIState( name )
            , m_damage( damage )
            , m_finished( false )
        {
        }


        void DamageState::Enter(AIStateMachine *stateMachine)
        {
            auto animator = stateMachine->GetEntity()->GetComponent<EntityAnimator>();

            auto dmgComp = stateMachine->GetEntity()->AddComponent<DamageOnCollide>();

            dmgComp->SetDamageToApply( m_damage );

            dmgComp->SetDamageInterval(1.0f);

            dmgComp->SetDeleteOnCollision(false);

            animator->Play( m_clipName );

            animator->Listener(this)
                .On(EntityAnimatorEvent::FinishedAnimating, &DamageState::onAnimationFinished);
        }

        void DamageState::Update(AIStateMachine *stateMachine)
        {
            // this is where we could check on animation and exit once done
        }

        void DamageState::Exit(AIStateMachine *stateMachine)
        {
            // delete that I don wanna do no mo dmg
            stateMachine->GetEntity()->RemoveComponent<DamageOnCollide>();
        }

        void DamageState::SetAnimationClip(const std::string& clip)
        {
            m_clipName = clip;
        }

        void DamageState::applyDamage(ursine::ecs::Entity* obj, const ursine::SVec3& contact, float damage)
        {
            Health* healthComp = obj->GetComponent< Health >();

            healthComp->DealDamage(contact, damage, false);

            // NOTE: we need to change a variable here to change from this state to the delay state
        }

        void DamageState::onAnimationFinished(EVENT_HANDLER(EntityAnimator))
        {
            EVENT_ATTRS(EntityAnimator, ursine::ecs::EntityEventArgs);

            m_finished = true;

            sender->Listener(this)
                .Off(EntityAnimatorEvent::FinishedAnimating, &DamageState::onAnimationFinished);
        }

    }
}
