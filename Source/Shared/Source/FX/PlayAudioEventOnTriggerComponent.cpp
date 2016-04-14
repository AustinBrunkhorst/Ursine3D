/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayAudioEventOnTriggerComponent.cpp
**
** Author:
** - JordanEllis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "PlayAudioEventOnTriggerComponent.h"

#include <EntityEvent.h>
#include <CollisionEventArgs.h>

NATIVE_COMPONENT_DEFINITION( PlayAudioEventOnTrigger );

using namespace ursine;
using namespace ecs;
using namespace resources;

PlayAudioEventOnTrigger::PlayAudioEventOnTrigger(void)
    : BaseComponent( ) { }

PlayAudioEventOnTrigger::~PlayAudioEventOnTrigger(void)
{
    GetOwner( )->Listener( this )
        .Off( ENTITY_COLLISION_PERSISTED, &PlayAudioEventOnTrigger::onTrigger );
}

void PlayAudioEventOnTrigger::OnSceneReady(Scene *scene)
{
    GetOwner( )->Listener( this )
        .On( ENTITY_COLLISION_PERSISTED, &PlayAudioEventOnTrigger::onTrigger );
}

const ResourceReference &PlayAudioEventOnTrigger::GetAudioEvent(void) const
{
    return m_event;
}

void PlayAudioEventOnTrigger::SetAudioEvent(const ResourceReference &sfx)
{
    m_event = sfx;

    NOTIFY_COMPONENT_CHANGED( "audioEvent", m_event );
}

void PlayAudioEventOnTrigger::onTrigger(EVENT_HANDLER(ursine::ecs::Entity))
{
    EVENT_ATTRS(Entity, physics::CollisionEventArgs);

    auto itr = triggerEntities.Find( args->otherEntity->GetName( ) );

    if (itr == triggerEntities.end( ))
        return;

    GetOwner( )->GetComponent<AudioEmitter>( )->PushEvent( m_event );

    GetOwner( )->Listener( this )
        .Off( ENTITY_COLLISION_PERSISTED, &PlayAudioEventOnTrigger::onTrigger );
}
