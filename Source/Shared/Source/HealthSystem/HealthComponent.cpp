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
#include "GameEvents.h"
#include "DamageEvent.h"
#include "AudioEmitterComponent.h"
#include "PlayerIdComponent.h"
#include "CollisionEventArgs.h"
#include "DamageOnCollideComponent.h"
#include "AbstractHitscanWeapon.h"
#include "UIEvents.h"
#include "UIScreensConfigComponent.h"
#include "GameEvents.h"

#include <Application.h>

NATIVE_COMPONENT_DEFINITION( Health );

using namespace ursine;
using namespace ecs;

Health::Health(void)
    : BaseComponent( )
    , EventDispatcher( this )
    , m_type( ENEMY_HEALTH )
    , m_health( 100 )
    , m_maxHealth( 100 )
    , m_shield( 100 )
    , m_maxShield( 100 )
    , m_deleteOnZero( false )
    , m_spawnOnDeath( false )
    , m_invulnerable( false )
    , m_hasShield( false )
{ }

Health::~Health(void)
{
    GetOwner( )->Listener(this)
        .Off( game::REVIVE_PLAYER, &Health::onRevive )
        .Off( ursine::ecs::ENTITY_REMOVED, &Health::onDeath );
}

HealthType Health::GetHealthType(void) const
{
    return m_type;
}

void Health::SetHealthType(HealthType type)
{
    m_type = type;

    NOTIFY_COMPONENT_CHANGED( "healthType", m_type );
}

float Health::GetHealth(void) const
{
    return m_health;
}

void Health::SetHealth(const float health)
{
    m_health = math::Max(health, 0.0f);

    NOTIFY_COMPONENT_CHANGED( "EntityHealth", m_health );

    auto *player = GetOwner( )->GetComponent<PlayerID>( );

    // dispacth to ui if player
    if (player)
    {
        ui_event::PlayerHealthUpdated healthEvent;

        healthEvent.playerID = player->GetID( );
        healthEvent.percent = m_health / m_maxHealth;

        auto world = GetOwner( )->GetWorld( );

        if (!world)
            return;

        auto settings = world->GetSettings( );

        if (!settings)
            return;

        auto *ui = settings->GetComponent<UIScreensConfig>( );

        if (ui)
            ui->TriggerPlayerHUDEvent( healthEvent );
    }
}

float Health::GetMaxHealth(void) const
{
    return m_maxHealth;
}

void Health::SetMaxHealth(float health)
{
    m_maxHealth = health;

    SetHealth( math::Min( m_health, m_maxHealth ) );
    NOTIFY_COMPONENT_CHANGED( "MaxHealth", m_maxHealth );
}

const ursine::resources::ResourceReference &Health::GetArchetypeOnDeath(void) const
{
    return m_objToSpawn;
}

void Health::SetArchetypeOnDeath(const ursine::resources::ResourceReference &objToSpawn)
{
    m_objToSpawn = objToSpawn;

    NOTIFY_COMPONENT_CHANGED( "archetypeToSpawnOnDeath", m_objToSpawn );
}

bool Health::GetDeleteOnZeroHealth(void) const
{
    return m_deleteOnZero;
}

void Health::SetDeleteOnZeroHealth(bool flag)
{
    m_deleteOnZero = flag;

    NOTIFY_COMPONENT_CHANGED( "deleteOnZeroHealth", m_deleteOnZero );
}

bool Health::GetSpawnOnDeath(void) const
{
    return m_spawnOnDeath;
}

void Health::SetSpawnOnDeath(bool state)
{
    m_spawnOnDeath = state;

    NOTIFY_COMPONENT_CHANGED( "SpawnOnDeath", m_spawnOnDeath );
}

bool Health::GetInvulnerable(void) const
{
    return m_invulnerable;
}

void Health::SetInvulnerable(bool invulnerable)
{
    m_invulnerable = invulnerable;

    NOTIFY_COMPONENT_CHANGED( "invulnerable", m_invulnerable );
}

bool Health::GetHasShield(void) const
{
    return m_hasShield;
}

void Health::SetHasShield(bool toggle)
{
    m_hasShield = toggle;

    NOTIFY_COMPONENT_CHANGED( "hasShield", m_hasShield );
}

float Health::GetShieldHealth(void) const
{
    return m_shield;
}

void Health::SetShieldHealth(float shield)
{
    m_shield = math::Max(shield, 0.0f);

    NOTIFY_COMPONENT_CHANGED( "ShieldHealth", m_shield );
}

float Health::GetMaxShieldHealth(void) const
{
    return m_maxShield;
}

void Health::SetMaxShieldHealth(float shield)
{
    m_maxShield = shield;

    SetShieldHealth( math::Min( shield, m_shield ) );

    NOTIFY_COMPONENT_CHANGED( "MaxShieldHealth", m_maxShield );
}

void Health::AddHealth(float healthToAdd)
{
    // make sure not to exceed max health
    SetHealth( math::Min( m_health + healthToAdd, m_maxHealth ) );
}

void Health::AddShieldHealth(float shieldToAdd)
{
    // make sure not to exceed max shield
    SetShieldHealth( math::Min( m_shield + shieldToAdd, m_maxShield ) );
}

void Health::DealDamage(const EntityHandle &damager, float damage)
{
    if (m_health <= 0 || m_invulnerable)
        return;

    auto owner = GetOwner( );
    auto *world = owner->GetWorld( );
    HealthEventArgs args( m_health, m_health / m_maxHealth, damager );

    Dispatch( HEALTH_DAMAGED, &args );

    // Message the UI
    if (GetOwner( )->HasComponent<PlayerID>( ))
        sendDamageUIEvent( damager, damage );

    // Check to see if we have a shield
    if (m_hasShield && m_shield > 0.0f)
    {
        auto newShieldHealth = m_shield - damage;

        // deal the damage to the shield
        SetShieldHealth( newShieldHealth );

        // early out while shield is still up
        if ( newShieldHealth > 0.0f )
            return;

        // carry over the damage to the health
        damage = -newShieldHealth;
    }

    SetHealth( GetHealth( ) - damage );

    args.health = m_health;
    args.percentage = m_health / m_maxHealth;

    if (m_health <= 0)
    {
        Dispatch( HEALTH_ZERO, &args );

        if (m_deleteOnZero)
            GetOwner( )->Delete( );
    }
    else
    {
        // dispatch damage taken event
        Dispatch( HEALTH_DAMAGE_TAKEN, &args );
    }
}

void Health::DealDamage(const EntityHandle &damager, const SVec3& contactPoint, float damage, bool crit)
{
    DealDamage(damager, damage);

    sendDamageTextEvent(contactPoint, damager, damage, crit);
}

bool Health::CanDamage(DamageOnCollide *damage) const
{
    auto type = damage->GetDamageType( );

    return type == DAMAGE_ALL || type == m_type;
}

bool Health::CanDamage(AbstractHitscanWeapon *weapon) const
{
    auto type = weapon->GetDamageType( );

    return type == DAMAGE_ALL || type == m_type;
}

void Health::OnInitialize(void)
{
    m_maxHealth = m_health;

    m_maxShield = m_shield;

    GetOwner( )->Listener( this )
        .On( game::REVIVE_PLAYER, &Health::onRevive )
        .On( ENTITY_REMOVED, &Health::onDeath );
}

void Health::sendDamageTextEvent(const SVec3& contact, const EntityHandle &damager, float damage, bool crit)
{
    EntityHandle owner = GetOwner( );

    game::DamageEventArgs dEvent( contact, owner, damager, damage, crit, m_invulnerable );

    owner->GetWorld( )->Dispatch( game::DAMAGE_TEXT_EVENT, &dEvent );
}

void Health::onDeath(EVENT_HANDLER(ursine::ecs::ENTITY_REMOVED))
{
    if (m_spawnOnDeath)
    {
        auto obj = GetOwner( )->GetWorld( )->CreateEntityFromArchetype( m_objToSpawn );

        if (obj)
            obj->GetTransform( )->SetWorldPosition( GetOwner( )->GetTransform( )->GetWorldPosition( ) );
    }
}

void Health::onRevive(EVENT_HANDLER(ursine::ecs::Entity))
{
    AddHealth( m_maxHealth );
}

void Health::sendDamageUIEvent(const EntityHandle &damager, float damage)
{
    ui_event::PlayerDamaged damageEvent;

    damageEvent.playerID = GetOwner( )->GetComponent<PlayerID>( )->GetID( );
    damageEvent.damage = damage;
    damageEvent.back = damageEvent.front = 
    damageEvent.left = damageEvent.right = false;

    auto trans = GetOwner( )->GetTransform( );
    auto damagerPos = damager->GetTransform( )->GetWorldPosition( );
    auto ourPos = trans->GetWorldPosition( );
    auto dir = damagerPos - ourPos;

    dir.Y( ) = 0.0f;

    dir.Normalize( );

    float xCos = trans->GetRight( ).Dot( dir );
    float yCos = trans->GetForward( ).Dot( dir );

    if (abs(xCos) < 0.5f)
    {
        // Dealing with front back case
        if (yCos > 0.0f)
            damageEvent.front = true;
        else
            damageEvent.back = true;
    }
    else
    {
        // Dealing with right left case
        if (xCos < 0.0f)
            damageEvent.left = true;
        else
            damageEvent.right = true;
    }

    auto world = GetOwner( )->GetWorld( );

    if (!world)
        return;

    auto settings = world->GetSettings( );

    if (!settings)
        return;

    auto *ui = settings->GetComponent<UIScreensConfig>( );

    if (ui)
        ui->TriggerPlayerHUDEvent( damageEvent );
}
