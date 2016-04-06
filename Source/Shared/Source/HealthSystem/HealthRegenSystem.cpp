
/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
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
        regenComp  = regenIt.second;

        // should be looking to regen shield
        if ( regenComp->GetTimer( ) < regenComp->GetWaitTime( ) )
        {
            // will wait timer finish this frame?
            if ( regenComp->GetTimer( ) + dt >= regenComp->GetWaitTime( ) )
            {
                CalculateHealthToAdd( healthComp, regenComp );
            }

            regenComp->IncrementTimer( dt );
            printf("wiating\n");
        }

        // wait timer has finished
        else if ( healthComp->GetShieldHealth( ) < healthComp->GetMaxShieldHealth( ) )
        {
            // add health
            healthComp->AddShieldHealth( regenComp->GetShieldToAdd( ) * dt );
            printf("Regening\n");
        }

        // shield is at full health
        else if ( healthComp->GetHealth( ) < healthComp->GetMaxHealth( ) )
        {
            healthComp->AddHealth( regenComp->GetHealthToAdd( ) * dt );
            printf("healing\n");
        }

    }
}

void HealthRegenSystem::CalculateHealthToAdd(Health* healthComp, HealthRegen* regenComp)
{
    float percent = healthComp->GetHealth( ) / healthComp->GetMaxHealth( );
    float missing = healthComp->GetMaxHealth( ) - healthComp->GetHealth( );

    float healthToAdd = missing / ( regenComp->GetHealthRegenTime( ) * ( 1 - percent ) );

    regenComp->SetHealthToAdd( healthToAdd );

    percent = healthComp->GetShieldHealth( ) / healthComp->GetMaxShieldHealth( );
    missing = healthComp->GetMaxShieldHealth( ) - healthComp->GetShieldHealth( );

    healthToAdd = missing / ( regenComp->GetShieldRegenTime( ) * ( 1 - percent ) );

    regenComp->SetShieldToAdd( healthToAdd );
}

