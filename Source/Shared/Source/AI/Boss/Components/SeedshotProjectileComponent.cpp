/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** SeedshotProjectileComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "SeedshotProjectileComponent.h"

#include <EntityEvent.h>

NATIVE_COMPONENT_DEFINITION( SeedshotProjectile );

using namespace ursine;
using namespace ecs;
using namespace resources;

SeedshotProjectile::SeedshotProjectile(void)
    : BaseComponent( ) { }

void SeedshotProjectile::OnSceneReady(ursine::Scene *scene)
{
    GetOwner( )->GetComponent<AudioEmitter>( )->PushEvent( m_seedshotSfx );

    GetOwner( )->Listener( this )
        .On( ENTITY_REMOVED, &SeedshotProjectile::onDeath );
}

const ResourceReference &SeedshotProjectile::GetSeedshotSfx(void) const
{
    return m_seedshotSfx;
}

void SeedshotProjectile::SetSeedshotSfx(const ResourceReference &seedshotSfx)
{
    m_seedshotSfx = seedshotSfx;

    NOTIFY_COMPONENT_CHANGED( "seedshotSfx", m_seedshotSfx );
}

const ResourceReference &SeedshotProjectile::GetSeedshotEndSfx(void) const
{
    return m_seedshotEndSfx;
}

void SeedshotProjectile::SetSeedshotEndSfx(const ResourceReference &seedshotEndSfx)
{
    m_seedshotEndSfx = seedshotEndSfx;

    NOTIFY_COMPONENT_CHANGED( "seedshotEndSfx", m_seedshotEndSfx );
}

const ResourceReference &SeedshotProjectile::GetDamageSfxEmitter(void) const
{
    return m_damageSfxEmitter;
}

void SeedshotProjectile::SetDamageSfxEmitter(const ResourceReference &gooDamageSfx)
{
    m_damageSfxEmitter = gooDamageSfx;

    NOTIFY_COMPONENT_CHANGED( "damageSfxEmitter", m_damageSfxEmitter );
}

void SeedshotProjectile::onDeath(EVENT_HANDLER(Entity))
{
    auto emitter = GetOwner( )->GetComponent<AudioEmitter>( );
    emitter->PushEvent( m_seedshotEndSfx );

    auto damage = GetOwner( )->GetWorld( )->CreateEntityFromArchetype(
        m_damageSfxEmitter
    );

    damage->GetTransform( )->SetWorldPosition( 
        GetOwner( )->GetTransform( )->GetWorldPosition( )
    );
}
