/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** VineSpawnState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "VineSpawnState.h"

#include "VineAIStateMachine.h"
#include "VineAIComponent.h"

#include <AnimatorComponent.h>
#include <EntityEvent.h>
#include <ParticleEmitterComponent.h>

using namespace ursine;
using namespace ecs;

VineSpawnState::VineSpawnState(void)
    : VineAIState( "Spawn" )
    , m_finished( false ) { }

void VineSpawnState::Enter(VineAIStateMachine *machine)
{
    auto vineAI = machine->GetAI( );
    auto animator = vineAI->GetAnimator( );

    animator->GetOwner( )->Listener( this )
        .On( ENTITY_ANIMATION_FINISH, &VineSpawnState::onAnimationFinished );

    animator->SetPlaying( true );
    animator->SetCurrentState( "Spike_Up" );

    machine->SetBool( VineAIStateMachine::IsHome, true );

    vineAI->SetHomeLocation( 
        vineAI->GetOwner( )->GetTransform( )->GetWorldPosition( )
    );

    // reset the spawn particles
    auto entity = vineAI->GetOwner( )->GetChildByName( vineAI->GetSpawnParticleEmitterName( ) );

    auto emitters = entity->GetComponentsInChildren<ParticleEmitter>( );

    for (auto &emitter : emitters)
        emitter->ResetSpawnCount(  );
}

void VineSpawnState::onAnimationFinished(EVENT_HANDLER(Entity))
{
    EVENT_ATTRS(Entity, ursine::EventArgs);

    m_finished = true;

    sender->Listener( this )
        .Off( ENTITY_ANIMATION_FINISH, &VineSpawnState::onAnimationFinished );
}
