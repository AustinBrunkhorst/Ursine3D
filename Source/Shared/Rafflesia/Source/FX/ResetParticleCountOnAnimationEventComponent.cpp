/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ResetParticleCountOnAnimationEventComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ResetParticleCountOnAnimationEventComponent.h"

#include <EntityEvent.h>

NATIVE_COMPONENT_DEFINITION( ResetParticleCountOnAnimationEvent );

using namespace ursine;
using namespace ecs;

ResetParticleCountOnAnimationEvent::ResetParticleCountOnAnimationEvent(void)
    : BaseComponent( ) { }

void ResetParticleCountOnAnimationEvent::OnSceneReady(Scene *scene)
{
    GetOwner( )->GetRoot( )->Listener( this )
        .On( ENTITY_ANIMATION_STATE_EVENT, &ResetParticleCountOnAnimationEvent::onAnimEvent );
}

void ResetParticleCountOnAnimationEvent::onAnimEvent(EVENT_HANDLER(Entity))
{
    EVENT_ATTRS(Entity, AnimatorStateEventArgs);

    if (args->state == animStateName &&
        args->message == animEventMessage)
    {
        GetOwner( )->GetComponent<ParticleEmitter>( )->ResetSpawnCount( );
    }
}
