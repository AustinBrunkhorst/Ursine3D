/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayAudioEventOnEntityAnimationFinishComponent.cpp
**
** Author:
** - JordanEllis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "PlayAudioEventOnEntityAnimationFinishComponent.h"

NATIVE_COMPONENT_DEFINITION( PlayAudioEventOnEntityAnimationFinish );

using namespace ursine;
using namespace ecs;
using namespace resources;

PlayAudioEventOnEntityAnimationFinish::PlayAudioEventOnEntityAnimationFinish(void)
    : BaseComponent( ) { }

PlayAudioEventOnEntityAnimationFinish::~PlayAudioEventOnEntityAnimationFinish(void)
{
    auto animator = GetOwner( )->GetComponent<EntityAnimator>( );
    
    if (animator)
        animator->Listener( this )
        .Off( EntityAnimatorEvent::FinishedAnimating, &PlayAudioEventOnEntityAnimationFinish::onAnimationFinish );
}

void PlayAudioEventOnEntityAnimationFinish::OnSceneReady(Scene *scene)
{
    GetOwner( )->GetComponent<EntityAnimator>( )->Listener( this )
        .On( EntityAnimatorEvent::FinishedAnimating, &PlayAudioEventOnEntityAnimationFinish::onAnimationFinish );
}

const ResourceReference &PlayAudioEventOnEntityAnimationFinish::GetAudioEvent(void) const
{
    return m_event;
}

void PlayAudioEventOnEntityAnimationFinish::SetAudioEvent(const ResourceReference &sfx)
{
    m_event = sfx;

    NOTIFY_COMPONENT_CHANGED( "audioEvent", m_event );
}

void PlayAudioEventOnEntityAnimationFinish::onAnimationFinish(EVENT_HANDLER(EntityAnimator))
{
    EVENT_SENDER( EntityAnimator, sender );

    GetOwner( )->GetComponent<AudioEmitter>( )->PushEvent( m_event );

    sender->Listener( this )
        .Off( EntityAnimatorEvent::FinishedAnimating, &PlayAudioEventOnEntityAnimationFinish::onAnimationFinish );
}
