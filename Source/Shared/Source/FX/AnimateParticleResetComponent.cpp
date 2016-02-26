/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimateParticleResetComponent.cpp
**
** Author:
** - JordanEllis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "AnimateParticleResetComponent.h"

#include <ParticleEmitterComponent.h>
#include <Application.h>

NATIVE_COMPONENT_DEFINITION(AnimateParticleReset);

using namespace ursine;
using namespace ecs;

AnimateParticleReset::AnimateParticleReset(void)
    : BaseComponent( )
    , m_startDuration( 2.0f )
    , m_endDuration( 0.5f )
    , m_timeToChange( 5.0f )
    , m_durationTimer( 0.0f )
    , m_changeTimer( 0.0f )
    , m_startOnAwake( true )
    , m_playing( false )
    , m_stopOnComplete( false )
    , m_easeFunction( ursine::ease::Type::Linear )
{
    
}

AnimateParticleReset::~AnimateParticleReset(void)
{
    auto world = GetOwner( )->GetWorld( );

    if (world)
    {
        world->Listener( this )
            .Off( WORLD_UPDATE, &AnimateParticleReset::onUpdate );
    }
}

void AnimateParticleReset::OnInitialize(void)
{
    GetOwner( )->GetWorld( )->Listener( this )
        .On( WORLD_UPDATE, &AnimateParticleReset::onUpdate );

    m_emitter = GetOwner( )->GetComponent<ParticleEmitter>( );

    if (m_startOnAwake)
        Play( );
}

float AnimateParticleReset::GetStartDuration(void) const
{
    return m_startDuration;
}

void AnimateParticleReset::SetStartDuration(float duration)
{
    m_startDuration = duration;
}

float AnimateParticleReset::GetEndDuration(void) const
{
    return m_endDuration;
}

void AnimateParticleReset::SetEndDuration(float duration)
{
    m_endDuration = duration;
}

float AnimateParticleReset::GetTimeToChange(void) const
{
    return m_timeToChange;
}

void AnimateParticleReset::SetTimeToChange(float duration)
{
    m_timeToChange = duration;
}

bool AnimateParticleReset::GetStartOnAwake(void) const
{
    return m_startOnAwake;
}

void AnimateParticleReset::SetStartOnAwake(bool flag)
{
    m_startOnAwake = flag;
}

bool AnimateParticleReset::GetStopOnComplete(void) const
{
    return m_stopOnComplete;
}

void AnimateParticleReset::SetStopOnComplete(bool flag)
{
    m_stopOnComplete = flag;
}

ursine::ease::Type AnimateParticleReset::GetEaseFunction(void) const
{
    return m_easeFunction;
}

void AnimateParticleReset::SetEaseFunction(ursine::ease::Type type)
{
    m_easeFunction = type;
}

void AnimateParticleReset::Play(void)
{
    m_playing = true;
    m_changeTimer = 0.0f;
    m_durationTimer = 0.0f;
}

void AnimateParticleReset::onUpdate(EVENT_HANDLER(World))
{
    if (!m_playing)
        return;

    auto dt = Application::Instance->GetDeltaTime( );

    m_durationTimer += dt;
    m_changeTimer += dt;

    float t = m_changeTimer / m_timeToChange;

    if (t >= 1.0f)
    {
        if (m_stopOnComplete)
            m_playing = false;

        t = 1.0f;
    }

    float duration = (1.0f - t) * m_startDuration + t * m_endDuration;

    if (m_durationTimer > duration)
    {
        m_emitter->ResetSpawnCount( );

        m_durationTimer = 0.0f;
    }
}
