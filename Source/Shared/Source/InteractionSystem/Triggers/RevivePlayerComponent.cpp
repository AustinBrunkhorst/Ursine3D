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
#include "GameEvents.h"

NATIVE_COMPONENT_DEFINITION( RevivePlayer ) ;

using namespace ursine;


RevivePlayer::RevivePlayer(void) :
    BaseComponent( )
{
}

RevivePlayer::~RevivePlayer(void)
{
    //GetOwner( )->Listener( this )
    //     .Off( game::FIRE_END, &BaseWeapon::TriggerReleased ); 
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
    NOTIFY_COMPONENT_CHANGED("Texture", m_texture);
}

void RevivePlayer::OnInitialize(void)
{
    GetOwner( )->GetComponent<Interactable>( )->SetUpInteractable(this, Interactable::END);
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
}

void RevivePlayer::Interact(const ursine::ecs::EntityHandle& entity)
{
    CommandQueue* queue = m_queues[ entity ];

    // if player is interacting with downed player
    if ( queue && queue->QueryCommand( game::INTERACT_COMMAND ) )
    {
        // grab time
        float* time = &m_times[ entity ];

        // update
        *time += ursine::Application::Instance->GetDeltaTime( );

        // revive player if time has been met
        if ( *time > m_reviveTime )
        {
            InteractionComplete( );
        }
    }
    
}

void RevivePlayer::StopInteraction(const ursine::ecs::EntityHandle& entity)
{
    if (entity && m_times.find( entity ) != m_times.end( ))
        m_times[ entity ] = 0.0f;
}

void RevivePlayer::InteractionComplete(void)
{
    GetOwner( )->GetRoot( )->Dispatch( game::REVIVE_PLAYER, ursine::ecs::EntityEventArgs::Empty );
    GetOwner( )->Delete( );
}
