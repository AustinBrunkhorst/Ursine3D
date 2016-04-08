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

void SeedshotProjectile::onDeath(EVENT_HANDLER(Entity))
{
    GetOwner( )->GetComponent<AudioEmitter>( )->PushEvent( m_seedshotEndSfx );
}
