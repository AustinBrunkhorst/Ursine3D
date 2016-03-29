
/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BuffSystem.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include "Precompiled.h"
#include "BuffSystem.h"
#include "Application.h"
#include "BuffComponent.h"


ENTITY_SYSTEM_DEFINITION( BuffSystem ) ;

using namespace ursine;
using namespace ursine::ecs;

namespace
{
} // unnamed namespace


//////////////////////////////
////  Base Weapon System  ////
//////////////////////////////

BuffSystem::BuffSystem(ursine::ecs::World* world)
    : FilterSystem( world, Filter( ).One< BuffComponent >( ) )
{
}


void BuffSystem::Enable(ursine::ecs::Entity* entity)
{
    auto uniqueID = entity->GetUniqueID( );

    // grab all comps needed
    if ( entity->HasComponent< BuffComponent >( ) )
        m_buffComps[uniqueID] = entity->GetComponent< BuffComponent >( );
}

void BuffSystem::Disable(ursine::ecs::Entity* entity)
{
    auto uniqueID = entity->GetUniqueID( );

    m_buffComps.erase( uniqueID );
}

void BuffSystem::onUpdate(EVENT_HANDLER(World))
{
    float dt = Application::Instance->GetDeltaTime( );

    for ( auto buffComp : m_buffComps )
    {
        buffComp.second->UpdateBuffTimes( dt );
    }
}
