/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayClipOnAnimationFinishComponent.cpp
**
** Author:
** - JordanEllis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "PlayClipOnAnimationFinishComponent.h"

NATIVE_COMPONENT_DEFINITION( PlayClipOnAnimationFinish );

PlayClipOnAnimationFinish::PlayClipOnAnimationFinish(void)
    : BaseComponent( )
    , m_unsubscribed( true ) { }

PlayClipOnAnimationFinish::~PlayClipOnAnimationFinish(void)
{
    auto animator = GetOwner( )->GetComponent<EntityAnimator>( );

    if (animator && !m_unsubscribed)
        animator->Listener( this )
            .Off( EntityAnimatorEvent::FinishedAnimating, &PlayClipOnAnimationFinish::onAnimationFinish );
}

void PlayClipOnAnimationFinish::OnSceneReady(ursine::Scene *scene)
{
    auto animator = GetOwner( )->GetComponent<EntityAnimator>( );

    UAssert( animator, "Error: Why is this happening?" );

    animator->Listener( this )
        .On( EntityAnimatorEvent::FinishedAnimating, &PlayClipOnAnimationFinish::onAnimationFinish );

    m_unsubscribed = false;
}

const std::string &PlayClipOnAnimationFinish::GetClipName(void) const
{
    return m_clipName;
}

void PlayClipOnAnimationFinish::SetClipName(const std::string &name)
{
    m_clipName = name;
}

void PlayClipOnAnimationFinish::onAnimationFinish(EVENT_HANDLER(EntityAnimator))
{
    EVENT_SENDER(EntityAnimator, sender);

    sender->Play( m_clipName );

    sender->Listener( this )
        .Off( EntityAnimatorEvent::FinishedAnimating, &PlayClipOnAnimationFinish::onAnimationFinish );

    m_unsubscribed = true;
}
