/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ProjectileSystem.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include "Precompiled.h"
#include "ProjectileSystem.h"
#include "ProjectileComponent.h"
#include "SystemIncludes.h"

ENTITY_SYSTEM_DEFINITION( ProjectileSystem );

ProjectileSystem::ProjectileSystem( ursine::ecs::World * world )
    : FilterSystem( world, ursine::ecs::Filter( ).All<Projectile>( ) )
{
}

void ProjectileSystem::Process( ursine::ecs::Entity * entity )
{
    entity->GetComponent<Projectile>( )->Update( ursine::Application::Instance->GetDeltaTime( ) );
}
