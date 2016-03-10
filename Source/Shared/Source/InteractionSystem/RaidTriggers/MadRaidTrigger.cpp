/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** MadRaidTriggerComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "MadRaidTrigger.h"
#include "CommandQueueComponent.h"
#include "PlayerIdComponent.h"
#include "BuffComponent.h"
#include "HealthComponent.h"
#include "World.h"
#include "WorldEvent.h"
#include "EntityEvent.h"
#include "Application.h"


namespace
{
    void GiveBuff(BuffComponent* buffComp,
                  BuffType raidEffect,
                  float modifier,
                  float duration = BuffComponent::BUFF_INFINITE)
    {

        //switch ( raidEffect )
        {
        //default:
            buffComp->AddBuff(raidEffect, modifier, duration);
        }
    }


} // unnamed namespace


MadRaidTrigger::MadRaidTrigger(void) :
    BaseRaidTrigger( ),
    m_madColor( 1.0f, 1.0f, 1.0f, 1.0f ),
    m_madTime( 10.0f ),
    m_dealDamageTime( 2.0f ),
    m_damage( 10.0f ),
    m_modifier( 1.0f ),
    m_madTimer( 0.0f ),
    m_triggerType( EMPOWER ),
    m_madSymbol( "MadTriggerSymbol" )
{ }

MadRaidTrigger::~MadRaidTrigger(void)
{

}


const ursine::Color& MadRaidTrigger::GetMadColor(void) const
{
    return m_madColor;
}

void MadRaidTrigger::SetMadColor(const ursine::Color& color)
{
    m_madColor = color;
}


const float MadRaidTrigger::GetMadTime(void) const
{
    return m_madTime;
}

void MadRaidTrigger::SetMadTime(const float time)
{
    m_madTime = time;
}


const float MadRaidTrigger::GetDamageTime(void) const
{
    return m_dealDamageTime;
}

void MadRaidTrigger::SetDamageTime(const float time)
{
    m_dealDamageTime = time;
}


const float MadRaidTrigger::GetDamage(void) const
{
    return m_damage;
}

void MadRaidTrigger::SetDamage(const float damage)
{
    m_damage = damage;
}


const float MadRaidTrigger::GetModifier(void) const
{
    return m_modifier;
}

void MadRaidTrigger::SetModifier(const float modifier)
{
    m_modifier = modifier;
}


const std::string& MadRaidTrigger::GetMadSymbol(void) const
{
    return m_madSymbol;
}

void MadRaidTrigger::SetMadSymbol(const std::string& symbol)
{
    m_madSymbol = symbol;
}


void MadRaidTrigger::StartInteraction(const CommandQueue* queue, ursine::ecs::EntityHandle &entity)
{
    auto owner = queue->GetOwner( );
    bool isPlayer = owner->HasComponent< PlayerID >( );
    bool hasBuff = owner->HasComponent< BuffComponent >( );
    
    if ( !isPlayer || !hasBuff )
        return;

    ChildStartInteractionLogic( );

    AddPlayer(owner, owner->GetComponent< PlayerID >( )->GetID( ));
}

void MadRaidTrigger::Interact(const CommandQueue* queue, ursine::ecs::EntityHandle &entity)
{
}

void MadRaidTrigger::StopInteraction(const CommandQueue* queue, ursine::ecs::EntityHandle &entity)
{
    auto owner = queue->GetOwner( );
    bool isPlayer = owner->HasComponent< PlayerID >( );

    if ( isPlayer )
    {
        // if trigger is not mad then only one player was interaction with
        //   thus change to interaction state
        if ( NOT_MAD )
            ChangeToInteractState( );

        RemovePlayer( owner->GetComponent< PlayerID >( )->GetID( ) );

        ChildStopInteractionLogic( );
    }
}


void MadRaidTrigger::ChangeToMadState(int newID)
{
    // set mad timer
    m_madTimer = m_madTime;

    // set curr effect
    m_currEffect = TRIGGER_DAMAGE;

    // remove player effect from first on trigger
    if ( newID != -1 )
        ChangeEffect( (newID + 1) % 2 );

    // change color of particle
    SetAnimatorColors( m_madColor );
}

void MadRaidTrigger::ChangeToInteractState(void)
{
    // set current effect
    m_currEffect = m_raidEffect;

    // change color of particle
    SetAnimatorColors( m_interactColor );
}

void MadRaidTrigger::ChangeToSuccessState(void)
{
    SetAnimatorColors( m_successColor );
}


void MadRaidTrigger::AddPlayer(const ursine::ecs::EntityHandle &player, int id)
{
    // activate player
    m_players[ id ].init(player->GetComponent< BuffComponent>( ), m_currEffect, m_dealDamageTime);

    // inproprer interaction
    if ( m_playerCount && NOT_MAD )
        ChangeToMadState( id );

    // proper interaction (should only be one player on trigger)
    else if ( NOT_MAD )
        ChildSuccessLogic( id );

    GiveBuff( m_players[ id ].buffComp, m_currEffect, m_modifier );

    // connect to player's death event
    player->Listener( this )
        .On( ursine::ecs::ENTITY_REMOVED, &MadRaidTrigger::BaseRaidTrigger::onPlayerDeath );

    ++m_playerCount;
}


// we will only do this in the mad state
//   - this is why assuming TRIGGER_DAMAGE
void MadRaidTrigger::ChangeEffect(int id)
{
    // change effect on player
    m_players[ id ].changeEffect( TRIGGER_DAMAGE );

    GiveBuff( m_players[ id ].buffComp, TRIGGER_DAMAGE, m_modifier );
}


void MadRaidTrigger::DealDamage(const ursine::ecs::EntityHandle &player)
{
    // get health component
    if ( player->HasComponent< Health >( ) )
    {
        // deals le damaushe
        player->GetComponent< Health >( )->DealDamage(m_damage);
    }

}


void MadRaidTrigger::UpdateDamageTimes(float dt)
{
    for ( int i = 0; i < 2; ++i )
    {
        if ( !m_players[ i ].buffComp )
            continue;
        m_players[ i ].decrementTime( dt );

        // has player been active long enough to be damaged
        if ( m_players[ i ].activeTime <= 0.0f )
        {
            // deal damage
            DealDamage( m_players[ i ].buffComp->GetOwner( ) );
            
            // reset active time
            m_players[ i ].activeTime = m_dealDamageTime;
        }
    }
}


void MadRaidTrigger::onUpdate(EVENT_HANDLER(ursine::ecs::WORLD_UPDATE))
{
    float dt = ursine::Application::Instance->GetDeltaTime( );

    // player count is > 1 then trigger is in mad state
    if ( MAD )
    {
        // deal damage if time is met and have yet to do so
        if ( m_playerCount )
        {
            // update damage times
            UpdateDamageTimes( dt );

            // reset timer
            m_madTimer = m_madTime;
        }

        // dont decrement timer if players are in
        else
        {
            m_madTimer -= dt;
        }
        
        
        // mad time is up
        if ( NOT_MAD )
        {
            ChangeToInteractState( );
        }

    }
    
    ChildUpdate( );
}











