/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RevivePlayerComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>

#include "RevivePlayerComponent.h"
#include "ComponentIncludes.h"
#include "PlayerIdCOmponent.h"
#include "CommandQueueComponent.h"
#include "HealthComponent.h"
#include "PlayerDownedObjectComponent.h"
#include "GameEvents.h"
#include "UIScreensConfigComponent.h"

#include <AudioEmitterComponent.h>

NATIVE_COMPONENT_DEFINITION( RevivePlayer ) ;

using namespace ursine;
using namespace ecs;

RevivePlayer::RevivePlayer(void) :
    BaseComponent( )
{
}

RevivePlayer::~RevivePlayer(void)
{
    GetOwner( )->Listener(this)
        .On( game::REVIVE_PLAYER, &RevivePlayer::OnReviveCheat );
}

float RevivePlayer::GetReviveTime(void) const
{
    return m_reviveTime;
}

void  RevivePlayer::SetReviveTime(float time)
{
    m_reviveTime = time;
}

const ursine::resources::ResourceReference& RevivePlayer::GetTexture(void) const
{
    return m_texture;
}

void RevivePlayer::SetTexture(const ursine::resources::ResourceReference& texture)
{
    m_texture = texture;
    NOTIFY_COMPONENT_CHANGED( "Texture", m_texture );
}

void RevivePlayer::OnInitialize(void)
{
    GetOwner( )->GetComponent<Interactable>( )->SetUpInteractable(this, Interactable::END);

    GetOwner( )->Listener( this )
        .On( game::REVIVE_PLAYER_CHEAT, &RevivePlayer::OnReviveCheat );
}

void RevivePlayer::StartInteraction(const ursine::ecs::EntityHandle& entity)
{
    // only want to include interactions with players
    if ( GetOwner( )->IsDeleting( ) || !entity->HasComponent< PlayerID >( ) )
        return;

    // make sure player is alive
    if ( entity->HasComponent< Health >( ) && 
         entity->GetComponent< Health >( )->GetHealth( ) <= 0.0f )
    {
        return;
    }

    m_queues[ entity ] = entity->GetComponent< CommandQueue >( );

    if (m_times.find( entity ) == m_times.end( ))
        m_times[ entity ] = 0.0f;

    messageUIToggle( entity, true );
}

void RevivePlayer::Interact(const ursine::ecs::EntityHandle& entity)
{
    CommandQueue* queue = m_queues[ entity ];

    // if player is interacting with downed player
    if ( queue && queue->QueryCommand( game::INTERACT_COMMAND ) )
    {
        // grab time
        float* time = &m_times[ entity ];

        if (math::IsZero( fmod( *time, 1.5f ) ))
        {
            revivingSfx( entity );
        }

        // update
        *time += Application::Instance->GetDeltaTime( );

        messageUIProgress( queue->GetOwner( ), *time );

        // revive player if time has been met
        if ( *time > m_reviveTime )
        {
            messageUISuccess(entity);

            InteractionComplete( );
        }
    }
    
}

void RevivePlayer::StopInteraction(const ursine::ecs::EntityHandle& entity)
{
    if (entity && m_times.find( entity ) != m_times.end( ))
        m_times[ entity ] = 0.0f;

    messageUIToggle( entity, false );
}

void RevivePlayer::InteractionComplete(void)
{
    GetOwner( )->Delete( );
    GetOwner( )->GetRoot( )->Dispatch( game::REVIVE_PLAYER, ursine::ecs::EntityEventArgs::Empty );
    revivedSfx( );
}

void RevivePlayer::OnReviveCheat(EVENT_HANDLER(ursine::ecs::Entity))
{
    EVENT_ATTRS( ursine::ecs::Entity, ursine::EventArgs );

    messageUISuccess( ursine::ecs::EntityHandle( nullptr ) );

    InteractionComplete( );
}

void RevivePlayer::messageUIToggle(const ursine::ecs::EntityHandle &reviver, bool toggle)
{
    auto world = GetOwner( )->GetWorld( );

    if (!world)
        return;

    auto settings = world->GetSettings( );

    if (!settings)
        return;

    auto ui = settings->GetComponent<UIScreensConfig>( );

    if (!ui)
        return;

    ui_event::PlayerReviveToggle event;

    event.toggle = toggle;
    event.playerReviving = reviver->GetComponent<PlayerID>( )->GetID( );
    event.playerDowned = GetOwner( )->GetRoot( )->GetComponent<PlayerID>( )->GetID( );

    ui->TriggerPlayerHUDEvent( event );
}

void RevivePlayer::messageUIProgress(const EntityHandle &reviver, float time)
{
    auto world = GetOwner( )->GetWorld( );

    if (!world)
        return;

    auto settings = world->GetSettings( );

    if (!settings)
        return;

    auto ui = settings->GetComponent<UIScreensConfig>( );

    if (!ui)
        return;

    ui_event::PlayerReviveUpdate event;

    event.percent = math::Min( time / m_reviveTime, 1.0f );
    event.playerReviving = reviver->GetComponent<PlayerID>( )->GetID( );
    event.playerDowned = GetOwner( )->GetRoot( )->GetComponent<PlayerID>( )->GetID( );

    ui->TriggerPlayerHUDEvent( event );
}

void RevivePlayer::messageUISuccess(const EntityHandle &)
{
    auto world = GetOwner( )->GetWorld( );

    if (!world)
        return;

    auto settings = world->GetSettings( );

    if (!settings)
        return;

    auto ui = settings->GetComponent<UIScreensConfig>( );

    if (!ui)
        return;

    ui_event::PlayerReviveSuccess event;

    event.playerRevived = GetOwner( )->GetRoot( )->GetComponent<PlayerID>( )->GetID( );

    if ( event.playerRevived == 1 )
        event.playerReviving = 0;

    else
        event.playerReviving = 1;
    

    ui->TriggerPlayerHUDEvent( event );
}

void RevivePlayer::revivingSfx(const EntityHandle &reviver)
{
    auto root = reviver->GetRoot( );

    root->GetComponent<AudioEmitter>( )->PushEvent(
        root->GetComponent<PlayerDownedObject>( )->GetRevivingSfx( )
    );
}

void RevivePlayer::revivedSfx(void)
{
    auto root = GetOwner( )->GetRoot( );

    root->GetComponent<AudioEmitter>( )->PushEvent(
        root->GetComponent<PlayerDownedObject>( )->GetBeingRevivedSfx( )
    );
}
