#include "Precompiled.h"

#include <iostream>

#include <HealthSystem/HealthComponent.h>
#include "CollisionEventArgs.h"
#include "EntityEvent.h"

#include "AIStatemachine.h"
#include "DamageState.h"
#include <AI/AIArchetypes/Fodder/FodderAIComponent.h>
#include <PlayerLogic/PlayerIDComponent.h>


using namespace ursine::ecs;
namespace ursine
{
    namespace sm
    {
        DamageState::DamageState(std::string name, float damage)
            : AIState( name )
            , m_damage( damage )
        {
        }


        void DamageState::Enter(AIStateMachine *stateMachine)
        {
            // in the future we could play an animation here
            std::cout << "Entered Damage State" << std::endl;
        }
        void DamageState::Update(AIStateMachine *stateMachine)
        {
            // this is where we could check on animation and exit once done
            std::cout << "Updated Damage State" << std::endl;
        }
        void DamageState::Exit(AIStateMachine *stateMachine)
        {
            // once we exit we can actually deal the damage
            std::cout << "Exited Damage State" << std::endl;

        }

        void DamageState::applyDamage(ursine::ecs::Entity* obj, const ursine::SVec3& contact, float damage)
        {
            Health* healthComp = obj->GetComponent< Health >();

            healthComp->DealDamage(contact, damage, false);

            // NOTE: we need to change a variable here to change from this state to the delay state
        }



    }
}
