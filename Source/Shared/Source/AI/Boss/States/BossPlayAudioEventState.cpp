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

using namespace ursine;
using namespace ecs;
using namespace resources;

BossPlayAudioEventState::BossPlayAudioEventState(ResourceReference &resource)
    : BossAIState( "Play Audio Event" )
    , m_resource( resource ) { }

void BossPlayAudioEventState::Enter(BossAIStateMachine *machine)
{
    auto emitter = machine->GetBoss( )->GetOwner( )->GetComponent<AudioEmitter>( );

    emitter->PushEvent( m_resource );
}
