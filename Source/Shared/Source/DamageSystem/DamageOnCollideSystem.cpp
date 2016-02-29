
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
using namespace ursine::ecs;

namespace
{
} // unnamed namespace


//////////////////////////////////
////  DamageOnCollide System  ////
//////////////////////////////////

DamageOnCollideSystem::DamageOnCollideSystem(ursine::ecs::World* world)
    : FilterSystem( world, Filter( ).One< DamageOnCollide >( ) )
{
}


void DamageOnCollideSystem::Enable(ursine::ecs::Entity* entity)
{
    auto uniqueID = entity->GetUniqueID( );

    m_damage[ uniqueID ] = entity->GetComponent< DamageOnCollide >( );
}

void DamageOnCollideSystem::Disable(ursine::ecs::Entity* entity)
{
    auto uniqueID = entity->GetUniqueID( );

    m_damage.erase( uniqueID );
}

void DamageOnCollideSystem::onUpdate(EVENT_HANDLER(World))
{
    float dt = Application::Instance->GetDeltaTime( );

    for (auto damage : m_damage )
    {
        damage.second->DecrementDamageIntervalTimes( dt );
    }
}
