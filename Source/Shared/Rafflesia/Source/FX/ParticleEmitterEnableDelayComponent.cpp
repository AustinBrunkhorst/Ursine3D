/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleEmitterEnableDelayComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ParticleEmitterEnableDelayComponent.h"

NATIVE_COMPONENT_DEFINITION( ParticleEmitterEnableDelay );

using namespace ursine;
using namespace ecs;

ParticleEmitterEnableDelay::ParticleEmitterEnableDelay(void)
    : BaseComponent( )
    , m_delay( 0.0f )
    , m_timer( 0.0f ) { }

ParticleEmitterEnableDelay::~ParticleEmitterEnableDelay(void)
{
    auto world = GetOwner( )->GetWorld( );

    if (world)
        world->Listener( this )
            .Off( WORLD_UPDATE, &ParticleEmitterEnableDelay::onUpdate );
}

void ParticleEmitterEnableDelay::OnSceneReady(Scene *scene)
{
    GetOwner( )->GetWorld( )->Listener( this )
        .On( WORLD_UPDATE, &ParticleEmitterEnableDelay::onUpdate );

    m_timer = m_delay;
}

float ParticleEmitterEnableDelay::GetDelay(void) const
{
    return m_delay;
}

void ParticleEmitterEnableDelay::SetDelay(float delay)
{
    m_delay = delay;

    NOTIFY_COMPONENT_CHANGED( "delay", m_delay );

    m_timer = m_delay;
}

void ParticleEmitterEnableDelay::onUpdate(EVENT_HANDLER(World))
{
    EVENT_SENDER(World, sender);

    m_timer -= Application::Instance->GetDeltaTime( );

    if (m_timer < 0.0f)
    {
        GetOwner( )->GetComponent<ParticleEmitter>( )->SetEmitting( true );

        sender->Listener( this )
            .Off( WORLD_UPDATE, &ParticleEmitterEnableDelay::onUpdate );
    }
}
