/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerDownedObjectComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>

#include "PlayerDownedObjectComponent.h"

#include "ComponentIncludes.h"
#include "CharacterControllerComponent.h"
#include "PlayerDeathCameraAnimatorComponent.h"
#include "SlimePitComponent.h"
#include "SlimePitDeathSpawnComponent.h"
#include "UIScreensConfigComponent.h"
#include "PlayerIDComponent.h"
#include "LevelSegmentManagerComponent.h"

#include <SweptControllerComponent.h>
#include <AudioEmitterComponent.h>

NATIVE_COMPONENT_DEFINITION( PlayerDownedObject );

using namespace ursine;
using namespace ecs;
using namespace resources;

PlayerDownedObject::PlayerDownedObject(void) 
    : BaseComponent( ) { }

PlayerDownedObject::~PlayerDownedObject(void)
{
    GetOwner( )->Listener(this)
        .Off( ENTITY_HIERARCHY_SERIALIZED, &PlayerDownedObject::onHeirarchy )
        .Off( ENTITY_COLLISION_PERSISTED, &PlayerDownedObject::onCollision );
}

const ResourceReference& PlayerDownedObject::GetReviveObject(void) const
{
    return m_reviveObject;
}

void PlayerDownedObject::SetReviveObject(const ResourceReference& archetype)
{
    m_reviveObject = archetype;

    NOTIFY_COMPONENT_CHANGED( "ReviveObject", m_reviveObject );
}

const ResourceReference &PlayerDownedObject::GetDeathSfx(void) const
{
    return m_deathSfx;
}

void PlayerDownedObject::SetDeathSfx(const ResourceReference &sfx)
{
    m_deathSfx = sfx;

    NOTIFY_COMPONENT_CHANGED( "deathSfx", m_deathSfx );
}

const ResourceReference &PlayerDownedObject::GetWhileDeadSfx(void) const
{
    return m_whileDeadSfx;
}

void PlayerDownedObject::SetWhileDeadSfx(const ResourceReference &sfx)
{
    m_whileDeadSfx = sfx;

    NOTIFY_COMPONENT_CHANGED( "whileDeadSfx", m_whileDeadSfx );
}

const ResourceReference &PlayerDownedObject::GetNotDeadSfx(void) const
{
    return m_notDeadSfx;
}

void PlayerDownedObject::SetNotDeadSfx(const ResourceReference &sfx)
{
    m_notDeadSfx = sfx;

    NOTIFY_COMPONENT_CHANGED( "notDeadSfx", m_notDeadSfx );
}

const ResourceReference &PlayerDownedObject::GetBeingRevivedSfx(void) const
{
    return m_beingRevivedSfx;
}

void PlayerDownedObject::SetBeingRevivedSfx(const ResourceReference &sfx)
{
    m_beingRevivedSfx = sfx;

    NOTIFY_COMPONENT_CHANGED( "beingRevivedSfx", m_beingRevivedSfx );
}

const ResourceReference &PlayerDownedObject::GetRevivingSfx(void) const
{
    return m_revivingSfx;
}

void PlayerDownedObject::SetRevivingSfx(const ResourceReference &sfx)
{
    m_revivingSfx = sfx;

    NOTIFY_COMPONENT_CHANGED( "revivingSfx", m_revivingSfx);
}

void PlayerDownedObject::OnInitialize(void)
{
    GetOwner( )->Listener( this )
        .On( ENTITY_HIERARCHY_SERIALIZED, &PlayerDownedObject::onHeirarchy )
        .On( ENTITY_COLLISION_PERSISTED, &PlayerDownedObject::onCollision );
}

void PlayerDownedObject::onHeirarchy(EVENT_HANDLER(ursine::ecs::Entity))
{
    Health *health = GetOwner( )->GetComponent<Health>( );

    if (health)
    {
        health->Listener( this )
            .On( HEALTH_ZERO, &PlayerDownedObject::onZeroHealth );
    }

    GetOwner( )->GetRoot( )->Listener( this )
        .On( game::REVIVE_PLAYER, &PlayerDownedObject::onRevive );
}

void PlayerDownedObject::onZeroHealth(EVENT_HANDLER(Health))
{
    EVENT_ATTRS(Health, HealthEventArgs);

    auto owner = GetOwner( );
    auto world = owner->GetWorld( );

    // Create the downed object
    auto obj = world->CreateEntityFromArchetype( m_reviveObject );

    if (!obj)
        return;

    auto trans = owner->GetTransform( );
    auto position = trans->GetWorldPosition( );

    obj->GetTransform( )->SetWorldPosition( trans->GetWorldPosition( ) );
    trans->AddChild( obj->GetTransform( ) );

    // Lock the player controller
    auto cc = owner->GetComponent<CharacterController>( );

    cc->SetLockLooking( true );
    cc->SetLockMovement( true );

    // Tell the swept controller it isn't active
    owner->GetComponent<SweptController>( )->SetEnableImpulses( false );

    // animate the camera
    auto animator = owner->GetComponentInChildren<PlayerDeathCameraAnimator>( );

    UAssert( animator, "Error: Need a death camera animator" );

    animator->AnimateDeath( );

    // turn off the hud for this player
    auto *ui = world->GetSettings( )->GetComponent<UIScreensConfig>( );

    UAssert( ui != nullptr,
        "UIConfig was null."    
    );

    ui_event::ToggleHUD e;

    e.toggled = false;
    e.playerID = owner->GetComponent<PlayerID>( )->GetID( );

    ui->TriggerPlayerHUDEvent( e );

    // If both players are dead, send event that both players are dead... lol
    auto players = world->GetEntitiesFromFilter( Filter( ).All<PlayerID>( ) );
    bool allDead = true;

    for (auto &player : players)
        if (player->GetComponent<Health>( )->GetHealth( ) > 0.0f)
            allDead = false;

    if (allDead)
    {
        ui_event::BothPlayersDied bothDeadEvent;

        // get the current segment
        auto sm = world->GetEntitiesFromFilter( Filter( ).All<LevelSegmentManager>( ) );

        bothDeadEvent.currentSegment = sm[ 0 ]->GetComponent<LevelSegmentManager>( )->GetCurrentSegment( );

        ui->TriggerPlayerHUDEvent( bothDeadEvent );
    }

    auto audioEmitter = owner->GetComponent<AudioEmitter>( );

    audioEmitter->PushEvent( m_deathSfx );
    audioEmitter->PushEvent( m_whileDeadSfx );
}

void PlayerDownedObject::onRevive(EVENT_HANDLER(Entity))
{
    auto owner = GetOwner( );

    // animate the camera
    auto animator = owner->GetComponentInChildren<PlayerDeathCameraAnimator>( );

    UAssert( animator, "Error: Need a death camera animator" );

    animator->AnimateRevival( );

    // turn on the hud for this player
    auto *ui = owner->GetWorld( )->GetSettings( )->GetComponent<UIScreensConfig>( );

    UAssert( ui != nullptr,
        "UIConfig was null."    
    );

    ui_event::ToggleHUD e;

    e.toggled = true;
    e.playerID = owner->GetComponent<PlayerID>( )->GetID( );

    ui->TriggerPlayerHUDEvent( e );

    auto audioEmitter = owner->GetComponent<AudioEmitter>( );

    audioEmitter->PushEvent( m_notDeadSfx );
}

void PlayerDownedObject::onCollision(EVENT_HANDLER(Entity))
{
    auto health = GetOwner( )->GetComponent<Health>( );

    if (health->GetHealth( ) > 0.0f)
        return;

    EVENT_ATTRS(Entity, physics::CollisionEventArgs);

    // Check to see if the damager that killed the player is a slime pit
    if (args->otherEntity->GetRoot( )->HasComponent<SlimePit>( ))
    {
        auto world = GetOwner( )->GetWorld( );
        auto trans = GetOwner( )->GetTransform( );
        auto position = trans->GetWorldPosition( );

        // move the player to the closest slime pit spawn position
        auto spawners = world->GetEntitiesFromFilter( Filter( ).All<SlimePitDeathSpawn>( ) );
        float minDist = std::numeric_limits<float>( ).max( );
        EntityHandle spawn = nullptr;

        for (auto &spawner : spawners)
        {
            auto dist = SVec3::DistanceSquared( spawner->GetTransform( )->GetWorldPosition( ), position );

            if (dist < minDist)
            {
                minDist = dist;
                spawn = spawner;
            }
        }

        if (spawn)
        {
            trans->SetWorldPosition(
                spawn->GetTransform( )->GetWorldPosition( )
            );
        }
    }
}

#if defined(URSINE_WITH_EDITOR)

void PlayerDownedObject::sendReviveEvent(void)
{
    GetOwner( )->Dispatch( game::REVIVE_PLAYER, EventArgs::Empty );
}

#endif
