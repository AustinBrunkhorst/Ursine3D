/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossPlayAudioEventState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossPlayAudioEventState.h"

#include "BossAIStateMachine.h"
#include "BossAIComponent.h"

#include <AudioEmitterComponent.h>
#include <AudioManager.h>
#include <AudioItemEventData.h>
#include <World.h>
#include <LocalTimerManager.h>
#include <Scene.h>

using namespace ursine;
using namespace ecs;
using namespace resources;

BossPlayAudioEventState::BossPlayAudioEventState(
    ResourceReference &resource, ursine::TimeSpan delay
)
    : BossAIState( "Play Audio Event" )
    , m_resource( resource )
    , m_delay( delay ) { }

void BossPlayAudioEventState::Enter(BossAIStateMachine *machine)
{
    auto boss = machine->GetBoss( )->GetOwner( );
    auto emitter = boss->GetComponent<AudioEmitter>( );

    boss->GetTimers( ).Create( m_delay ).Completed( [=] {
        emitter->PushEvent( m_resource );
    } );
}
