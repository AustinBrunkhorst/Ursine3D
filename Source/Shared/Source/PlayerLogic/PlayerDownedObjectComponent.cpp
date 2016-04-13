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

#include <SweptControllerComponent.h>

NATIVE_COMPONENT_DEFINITION( PlayerDownedObject );

using namespace ursine;
using namespace ecs;

PlayerDownedObject::PlayerDownedObject(void) 
    : BaseComponent( ) { }

PlayerDownedObject::~PlayerDownedObject(void)
{
    GetOwner( )->Listener(this)
        .Off( ENTITY_HIERARCHY_SERIALIZED, &PlayerDownedObject::onHeirarchy );
}

const resources::ResourceReference& PlayerDownedObject::GetReviveObject(void) const
{
    return m_reviveObject;
}

void PlayerDownedObject::SetReviveObject(const resources::ResourceReference& archetype)
{
    m_reviveObject = archetype;

    NOTIFY_COMPONENT_CHANGED( "ReviveObject", m_reviveObject );
}

void PlayerDownedObject::OnInitialize(void)
{
    GetOwner( )->Listener( this )
        .On( ENTITY_HIERARCHY_SERIALIZED, &PlayerDownedObject::onHeirarchy );
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

    // Check to see if the damager that killed the player is a slime pit
    if (args->damager && args->damager->GetRoot( )->HasComponent<SlimePit>( ))
    {
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

    // turn off the hud for this player
    auto *ui = world->GetSettings( )->GetComponent<UIScreensConfig>( );

    UAssert( ui != nullptr,
        "UIConfig was null."    
    );

    ui_event::ToggleHUD e;

    e.toggled = false;
    e.playerID = owner->GetComponent<PlayerID>( )->GetID( );

    ui->TriggerPlayerHUDEvent( e );
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
}

#if defined(URSINE_WITH_EDITOR)

void PlayerDownedObject::sendReviveEvent(void)
{
    GetOwner( )->Dispatch( game::REVIVE_PLAYER, EventArgs::Empty );
}

#endif
