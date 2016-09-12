/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayAudioEventComponent.h
**
** Author:
** - JordanEllis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "PlayAudioEventComponent.h"

using namespace ursine;
using namespace ecs;
using namespace resources;

NATIVE_COMPONENT_DEFINITION( PlayAudioEvent );

PlayAudioEvent::PlayAudioEvent(void)
    : BaseComponent( )
    , m_playOnAwake( true ) { }

void PlayAudioEvent::OnSceneReady(Scene *scene)
{
    if (m_playOnAwake)
        GetOwner( )->GetWorld( )->Listener( this )
            .On( WORLD_UPDATE, &PlayAudioEvent::onUpdate );
}

const ResourceReference &PlayAudioEvent::GetAudioEvent(void) const
{
    return m_event;
}

void PlayAudioEvent::SetAudioEvent(const ResourceReference &audioEvent)
{
    m_event = audioEvent;

    NOTIFY_COMPONENT_CHANGED( "audioEvent", m_event );
}

bool PlayAudioEvent::GetPlayOnAwake(void) const
{
    return m_playOnAwake;
}

void PlayAudioEvent::SetPlayOnAwake(bool toggle)
{
    m_playOnAwake = toggle;

    NOTIFY_COMPONENT_CHANGED( "playOnAwake", m_playOnAwake );
}

void PlayAudioEvent::Play(void)
{
    GetOwner( )->GetComponent<AudioEmitter>( )->PushEvent( m_event );
}

void PlayAudioEvent::onUpdate(EVENT_HANDLER(World))
{
    GetOwner( )->GetComponent<AudioEmitter>( )->PushEvent( m_event );

    GetOwner( )->GetWorld( )->Listener( this )
        .Off( WORLD_UPDATE, &PlayAudioEvent::onUpdate );
}
