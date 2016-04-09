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

void PlayerDownedObject::onZeroHealth(EVENT_HANDLER(Entity))
{
    // Create the downed object
    auto obj = GetOwner( )->GetWorld( )->CreateEntityFromArchetype( m_reviveObject );

    if (!obj)
        return;

    obj->GetTransform( )->SetWorldPosition( GetOwner( )->GetTransform( )->GetWorldPosition( ) );
    GetOwner( )->GetTransform( )->AddChild( obj->GetTransform( ) );

    // Lock the player controller
    auto cc = GetOwner( )->GetComponent<CharacterController>( );

    cc->SetLockLooking( true );
    cc->SetLockMovement( true );

    // Tell the swept controller it isn't active
    GetOwner( )->GetComponent<SweptController>( )->SetEnableImpulses( false );

    // animate the camera
    auto animator = GetOwner( )->GetComponentInChildren<PlayerDeathCameraAnimator>( );

    UAssert( animator, "Error: Need a death camera animator" );

    animator->AnimateDeath( );
}

void PlayerDownedObject::onRevive(EVENT_HANDLER(Entity))
{
    // animate the camera
    auto animator = GetOwner( )->GetComponentInChildren<PlayerDeathCameraAnimator>( );

    UAssert( animator, "Error: Need a death camera animator" );

    animator->AnimateRevival( );
}

#if defined(URSINE_WITH_EDITOR)

void PlayerDownedObject::sendReviveEvent(void)
{
    GetOwner( )->Dispatch( game::REVIVE_PLAYER, EventArgs::Empty );
}

#endif
