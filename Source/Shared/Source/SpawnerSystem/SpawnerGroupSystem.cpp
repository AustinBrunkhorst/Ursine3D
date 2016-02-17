/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SpawnerGroupSystem.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - CJ Payne
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "SpawnerGroupSystem.h"

#include "SpawnerGroupComponent.h"
#include "SpawnerComponent.h"

ENTITY_SYSTEM_DEFINITION( SpawnerGroupSystem );

using namespace ursine;
using namespace ecs;

SpawnerGroupSystem::SpawnerGroupSystem(World *world)
    : EntitySystem( world )
{
}

void SpawnerGroupSystem::OnInitialize(void)
{
    m_world->Listener( this )
        .On( WORLD_UPDATE, &SpawnerGroupSystem::onUpdate )
        .On( WORLD_ENTITY_COMPONENT_ADDED, &SpawnerGroupSystem::onComponentAdded )
        .On( WORLD_ENTITY_COMPONENT_REMOVED, &SpawnerGroupSystem::onComponentRemoved );
}

void SpawnerGroupSystem::OnRemove(void)
{
    m_world->Listener( this )
        .Off( WORLD_UPDATE, &SpawnerGroupSystem::onUpdate )
        .Off( WORLD_ENTITY_COMPONENT_ADDED, &SpawnerGroupSystem::onComponentAdded )
        .Off( WORLD_ENTITY_COMPONENT_REMOVED, &SpawnerGroupSystem::onComponentRemoved );
}

void SpawnerGroupSystem::OnAfterLoad(void)
{
    auto entities = m_world->GetEntitiesFromFilter( Filter( ).All<SpawnerGroup>( ) );

    for (auto &entity : entities)
    {
        // Add the spawner group to our internal array so we can update it
        auto group = entity->GetComponent<SpawnerGroup>( );
        auto itr = std::find( m_spawnerGroups.begin( ), m_spawnerGroups.end( ), group );

        if (itr == m_spawnerGroups.end( ))
            m_spawnerGroups.push_back( group );

        // Add all the group's children (Spawners) to itself
        auto spawners = group->GetOwner( )->GetComponentsInChildren<Spawner>( );

        for (auto spawner : spawners)
        {
            group->addSpawner( spawner );
        }
    }
}

void SpawnerGroupSystem::onUpdate(EVENT_HANDLER(World))
{
    for (auto &group : m_spawnerGroups)
    {
        // all that logic up in this bitch
    }
}

void SpawnerGroupSystem::onComponentAdded(EVENT_HANDLER(World))
{
    EVENT_ATTRS(World, ComponentEventArgs);

    // If a spawner group component has been added, add it to our system
    if (args->component->Is<SpawnerGroup>( ))
    {
        m_spawnerGroups.push_back( args->entity->GetComponent<SpawnerGroup>( ) );
    }
}

void SpawnerGroupSystem::onComponentRemoved(EVENT_HANDLER(World))
{
    EVENT_ATTRS(World, ComponentEventArgs);

    // If a spawner group component has been added, remove it from our system
    if (args->component->Is<SpawnerGroup>( ))
    {
        auto group = reinterpret_cast<SpawnerGroup*>( args->component );

        m_spawnerGroups.erase(
            find( m_spawnerGroups.begin( ), m_spawnerGroups.end( ), group )
        );

        // Find all children that are spawners, and delete them
        auto spawners = group->GetOwner( )->GetComponentsInChildren<Spawner>( );

        for (auto &spawner : spawners)
        {
            spawner->GetOwner( )->Delete( );
        }
    }
    // If a spawner has been removed, it means the entity was deleted.
    // Remove the spawner from it's group.
    else if (args->component->Is<Spawner>( ))
    {
        auto spawner = reinterpret_cast<Spawner*>( args->component );

        // Iterate through all groups, and see if they have the spawner.
        // If so, remove it.  We can't check for the parents because
        // when the entity is deleted, it is already removed from the hiearchy.
        for (auto group : m_spawnerGroups)
        {
            auto enemyType = spawner->GetEnemyType( );

            if (group->haveSpawnerOfType( enemyType ) &&
                group->m_spawners[ enemyType ] == spawner)
            {
                group->removeSpawner( spawner );
            }
        }
    }
}
