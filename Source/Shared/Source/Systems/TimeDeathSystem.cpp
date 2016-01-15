/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TimeDeathSystem.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include "Precompiled.h"
#include "TimeDeathSystem.h"
#include "TimeDeathComponent.h"
#include "SystemIncludes.h"

ENTITY_SYSTEM_DEFINITION( TimeDeathSystem );

TimeDeathSystem::TimeDeathSystem( ursine::ecs::World * world ) :
    FilterSystem( world, ursine::ecs::Filter( ).All<TimeDeath>( ) )
{
}

void TimeDeathSystem::Process( ursine::ecs::Entity * entity )
{
    entity->GetComponent<TimeDeath>( )->DecrementTimer( ursine::Application::Instance->GetDeltaTime( ) );
}

