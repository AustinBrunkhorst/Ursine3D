
/* ----------------------------------------------------------------------------
** Team Bear King
** � 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** HealthRegenSystem.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include "Precompiled.h"
#include "HealthRegenSystem.h"

#include "HealthRegenComponent.h"
#include "HealthComponent.h"

#include "Application.h"


ENTITY_SYSTEM_DEFINITION( HealthRegenSystem ) ;

using namespace ursine;
using namespace ursine::ecs;

namespace
{
} // unnamed namespace


//////////////////////////////
////  Base Weapon System  ////
//////////////////////////////

HealthRegenSystem::HealthRegenSystem(ursine::ecs::World* world)
    : FilterSystem( world, Filter( ).All< HealthRegen, Health >( ) )
{
}


void HealthRegenSystem::Enable(const ursine::ecs::EntityHandle& entity)
{
    m_healthRegens[ entity ] = entity->GetComponent< HealthRegen >( );
    m_healthComps[ entity ]  = entity->GetComponent< Health >( );
}

void HealthRegenSystem::Disable(const ursine::ecs::EntityHandle& entity)
{
    m_healthRegens.erase( entity );
    m_healthComps.erase( entity );
}

void HealthRegenSystem::onUpdate(EVENT_HANDLER(World))
{
    float dt = Application::Instance->GetDeltaTime( );

    HealthRegen* regenComp = nullptr;
    Health* healthComp = nullptr;

    for ( auto regenIt : m_healthRegens )
    {
        healthComp = m_healthComps[ regenIt.first ];

        if ( healthComp->GetHealth( ) <= 0.0f )
            continue;

        regenComp  = regenIt.second;

        // should be looking to regen shield
        if ( regenComp->GetTimer( ) < regenComp->GetWaitTime( ) )
        {
            regenComp->IncrementTimer( dt );
        }

        // wait timer has finished
        else if ( healthComp->GetShieldHealth( ) < healthComp->GetMaxShieldHealth( ) )
        {
            // add health
            healthComp->AddShieldHealth( regenComp->GetShieldToAdd( ) * dt );
        }

        // shield is at full health
        else if ( healthComp->GetHealth( ) < healthComp->GetMaxHealth( ) )
        {
            healthComp->AddHealth( regenComp->GetHealthToAdd( ) * dt );
        }

    }
}
