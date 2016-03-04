
/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** DamageSystem.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include "Precompiled.h"
#include "DamageOnCollideSystem.h"
#include "Application.h"
#include "DamageOnCollideComponent.h"


ENTITY_SYSTEM_DEFINITION(DamageOnCollideSystem) ;

using namespace ursine;
using namespace ecs;

//////////////////////////////////
////  DamageOnCollide System  ////
//////////////////////////////////

DamageOnCollideSystem::DamageOnCollideSystem(ursine::ecs::World* world)
    : FilterSystem( world, Filter( ).One< DamageOnCollide >( ) )
{
}

void DamageOnCollideSystem::Enable(const ursine::ecs::EntityHandle &entity)
{
    m_damage[ entity ] = entity->GetComponent< DamageOnCollide >( );
}

void DamageOnCollideSystem::Disable(const ursine::ecs::EntityHandle &entity)
{
    m_damage.erase( entity );
}

void DamageOnCollideSystem::onUpdate(EVENT_HANDLER(World))
{
    float dt = Application::Instance->GetDeltaTime( );

    for (auto damage : m_damage )
    {
        damage.second->DecrementDamageIntervalTimes( dt );
    }
}
