/* ---------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BomberExplodeState.cpp
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
#include "BomberExplodeState.h"
#include <EntityAnimation/EntityAnimatorComponent.h>



namespace ursine
{
    namespace sm
    {
        BomberExplodeState::BomberExplodeState(const std::string &name, WalkState *walkState, float damage)
            : AIState(name)
            , m_walk(walkState)
            , m_finished(false)
        {
        }

        void BomberExplodeState::Enter(AIStateMachine* machine)
        {
            // start that dope explode animation

            m_walk->Enter(machine);
        }

        void BomberExplodeState::Update(AIStateMachine* machine)
        {
            // we gon start moving at the player, maybe speed up over time
            m_walk->Update(machine);

            auto aiTrans = machine->GetEntity()->GetTransform();
            Vec3 aiActorPos = aiTrans->GetWorldPosition();

            auto playerPos = GetTargetPlayerPosition(aiTrans->GetOwner()->GetWorld());

            auto playerDirection = playerPos - aiActorPos;

            if (playerDirection.Length() <= m_explodeRange)
            {
                m_finished = true;

                // we will just manually call exit
                Exit(machine);
            }
        }

        void BomberExplodeState::Exit(AIStateMachine* machine)
        {
            m_walk->Exit(machine);

            // if we are here, the animation ended so it is time to explode:
            
            // AOE damage
            // &
            // dope particles
            machine->GetEntity()->GetWorld()->CreateEntityFromArchetype(m_objToSpawn);

            // destroy ourselves (just going to use data already stored in walkstate)
            machine->GetEntity(  ).Get( )->Delete( );
        }

        void BomberExplodeState::SetExplosionArchetype(ursine::resources::ResourceReference& dmgExplosionArchetype)
        {
            m_objToSpawn = dmgExplosionArchetype;
        }

        void BomberExplodeState::SetExplodeRange(float range)
        {
            m_explodeRange = range;
        }
    }
}