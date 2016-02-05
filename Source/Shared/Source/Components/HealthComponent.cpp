/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** HealthComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "HealthComponent.h"

#include <SystemManager.h>
#include <Systems/RoundSystem.h>
#include "TeamComponent.h"
#include "GameEvents.h"
#include "DamageOnCollideComponent.h"
#include "AudioEmitterComponent.h"

NATIVE_COMPONENT_DEFINITION( Health );

namespace
{
    const std::string kTakeDamage = "PLAYER_TAKE_DAMAGE";
}

Health::Health(void)
    : BaseComponent( )
    , m_health( 100 )
    , m_objToSpawn( "" )
    , m_spawnOnDeath( false )
{
    
}

Health::~Health(void)
{
    GetOwner( )->Listener(this)
        .Off(ursine::ecs::ENTITY_REMOVED, &Health::OnDeath);
}

float Health::GetHealth(void) const
{
    return m_health;
}

void Health::SetHealth(const float health)
{
    m_health = health;
}

float Health::GetMaxHealth(void) const
{
    return m_maxHealth;
}

const std::string& Health::GetArchetypeOnDeath(void) const
{
    return m_objToSpawn;
}

void Health::SetArchetypeOnDeath(const std::string& objToSpawn)
{
    m_objToSpawn = objToSpawn;

    if ( m_objToSpawn.find(".uatype") == std::string::npos )
        m_objToSpawn += ".uatype";
}

bool Health::GetSpawnOnDeath(void) const
{
    return m_spawnOnDeath;
}
void Health::SetSpawnOnDeath(const bool state)
{
    m_spawnOnDeath = state;
}


void Health::DealDamage(const float damage)
{
    auto owner = GetOwner( );

    m_health -= damage;

    if (m_health <= 0)
    {
        GetOwner( )->Delete( );
    }

    URSINE_TODO("Fix sound hack for health");
    ursine::ecs::AudioEmitter* emitter = owner->GetComponent<ursine::ecs::AudioEmitter>( );

    if ( emitter )
        emitter->AddSoundToPlayQueue(kTakeDamage);
}

void Health::OnInitialize(void)
{
    m_maxHealth = m_health;

    GetOwner( )->Listener(this)
        .On(ursine::ecs::ENTITY_REMOVED, &Health::OnDeath);

    ConnectToAllCritSpots( );
}

void Health::ConnectToAllCritSpots(void)
{
    GetOwner( )->Listener(this).On( game::DAMAGE_EVENT, &Health::OnDamaged );

    ursine::ecs::Entity* entity;
    auto world = GetOwner( )->GetWorld( );
    auto children = GetOwner( )->GetChildren( );


    for ( auto it : *children )
    {
        entity = world->GetEntity( it );
        entity->Listener(this).On(game::DAMAGE_EVENT, &Health::OnDamaged);
    }
}


void Health::OnDamaged(EVENT_HANDLER(game::DAMAGE_EVENT))
{
    EVENT_ATTRS(ursine::ecs::Entity, game::DamageEventArgs);

    DealDamage(args->m_damage);

    args->m_damageComp->AddEntityToIntervals( GetOwner( )->GetUniqueID( ) );
}


void Health::OnDeath(EVENT_HANDLER(ursine::ecs::ENTITY_REMOVED))
{
    if ( m_spawnOnDeath )
    {
        ursine::ecs::Entity* obj = GetOwner( )->GetWorld( )->CreateEntityFromArchetype(m_objToSpawn);

        obj->GetTransform( )->SetWorldPosition( GetOwner( )->GetTransform( )->GetWorldPosition( ) );
    }
}


