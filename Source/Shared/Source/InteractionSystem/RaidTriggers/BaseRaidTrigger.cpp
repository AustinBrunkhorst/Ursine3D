/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BaseRaidTrigger.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "BaseRaidTrigger.h"
#include "BuffComponent.h"
#include "PlayerIdComponent.h"
#include "ParticleColorAnimatorComponent.h"
#include "World.h"
#include "EntityEvent.h"
#include "Color.h"


namespace
{
    ursine::Color PrepColor(const ursine::Color& color, const float percent)
    {
        ursine::Color c;
        
        c.b = color.b * percent;
        c.r = color.r * percent;
        c.g = color.g * percent;
        c.a = color.a * percent;

        return c;
    }
} //unnamed namespace



BaseRaidTrigger::BaseRaidTrigger(void) :
    m_players{ Player( ), Player( ) },
    m_playerCount( 0 ),
    m_raidEffect( EMPOWER ),
    m_currEffect( EMPOWER ),
    m_pAnimator( nullptr ),
    m_successColor( 1.0f, 1.0f, 1.0f, 1.0f ),
    m_interactColor( 1.0f, 1.0f, 1.0f, 1.0f ),
    m_uiSymbol( "UI_Default" ),
    m_archetypeToLoad( "FX/FX_TEST.uatype"),
    m_active( true )
{ }

BaseRaidTrigger::~BaseRaidTrigger(void)
{
    ClearPlayers( );
}


const BuffType BaseRaidTrigger::GetRaidEffect(void) const
{
    return m_raidEffect;
}

void BaseRaidTrigger::SetRaidEffect(const BuffType effect)
{
    m_raidEffect = effect;
    m_currEffect = effect;
}


const std::string BaseRaidTrigger::GetEmitter(void) const
{
    if ( !m_pAnimator )
        return std::string( "" );

    return m_pAnimator->GetOwner( )->GetName( );
}

void BaseRaidTrigger::SetEmitter(std::string archetype)
{
    if ( archetype.size( ) )
    {
        if ( archetype.find("FX/") == std::string::npos )
            archetype = "FX/" + archetype;

        if ( archetype.find(".uatype") == std::string::npos )
            archetype += ".uatype";

        m_archetypeToLoad = archetype;
    }
}


const ursine::Color& BaseRaidTrigger::GetSuccessColor(void) const
{
    return m_successColor;
}

void BaseRaidTrigger::SetSuccessColor(const ursine::Color& color)
{
    m_successColor = color;
}


const ursine::Color& BaseRaidTrigger::GetInteractColor(void) const
{
    return m_interactColor;
}

void BaseRaidTrigger::SetInteractColor(const ursine::Color& color)
{
    m_interactColor = color;
}


const std::string& BaseRaidTrigger::GetUISymbol(void)
{
    return m_uiSymbol;
}

void BaseRaidTrigger::SetUISymbol(const std::string& symbol)
{
    m_uiSymbol = symbol;
}



///////////////////////////////////////////////
////////       Protected Methods       ////////
///////////////////////////////////////////////

void BaseRaidTrigger::Initialize(ursine::ecs::Entity* owner)
{
    if ( m_archetypeToLoad.size( ) )
    {
        auto entity = owner->GetWorld( )
            ->CreateEntityFromArchetype(WORLD_ARCHETYPE_PATH + m_archetypeToLoad, m_archetypeToLoad);

        m_pAnimator = entity->GetComponent< ursine::ecs::ParticleColorAnimator >( );

        entity->GetTransform( )->SetWorldPosition(owner->GetTransform( )->GetWorldPosition( ));
        owner->GetTransform( )->AddChild(entity->GetTransform( ));
    }
}

void BaseRaidTrigger::SetAnimatorColors(const ursine::Color& color)
{
    m_pAnimator->SetColor0( PrepColor( color, 0.2f ) );
    m_pAnimator->SetColor1( PrepColor( color, 0.6f ) );
    m_pAnimator->SetColor2( PrepColor( color, 1.0f ) );
    m_pAnimator->SetColor3( PrepColor( color, 0.8f ) );
    m_pAnimator->SetColor4( PrepColor( color, 0.2f ) );
}


void BaseRaidTrigger::onDeath(EVENT_HANDLER(ursine::ecs::Entity))
{
    ClearPlayers( );
}


void BaseRaidTrigger::onPlayerDeath(EVENT_HANDLER(ENTITY_REMOVED))
{
    EVENT_ATTRS(ursine::ecs::Entity, ursine::ecs::EntityEventArgs);
    
    RemovePlayer( args->entity->GetComponent< PlayerID >( )->GetID( ) );
}



///////////////////////////////////////////////
////////        Player  Methods        ////////
///////////////////////////////////////////////

BaseRaidTrigger::Player::Player(void) :
    buffComp( nullptr ),
    activeTime( 0.0f)
{ }

void BaseRaidTrigger::Player::init(BuffComponent* buffComponent, BuffType effect, float time)
{
    buffComp = buffComponent;
    effectActive = effect;
    activeTime = time;
}

void BaseRaidTrigger::Player::uninit(void)
{
    if ( buffComp )
        buffComp->RemoveBuff( effectActive );
    
    buffComp = nullptr;
    activeTime = 0.0f;
}

void BaseRaidTrigger::Player::changeEffect(BuffType effect)
{
    // remove current buff
    buffComp->RemoveBuff( effectActive );
    
    // change active effect
    effectActive = effect;
}

void BaseRaidTrigger::Player::decrementTime(float dt)
{
    activeTime -= dt;
}


void BaseRaidTrigger::RemovePlayer(int id)
{
    if ( m_players[ id ].buffComp )
    {
        m_players[ id ].buffComp->GetOwner( )->Listener(this)
            .Off(ursine::ecs::ENTITY_REMOVED, &BaseRaidTrigger::onPlayerDeath);
    }

    m_players[ id ].uninit( );

    --m_playerCount;
}

void BaseRaidTrigger::ClearPlayers(void)
{
    RemovePlayer( 0 );
    RemovePlayer( 1 );
}



