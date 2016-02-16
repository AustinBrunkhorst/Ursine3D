/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SpawnerGroupComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - CJ Payne
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "SpawnerGroupComponent.h"

SpawnerGroup::SpawnerGroup(void)
    : BaseComponent( )
    , m_enemyType( AIArchetype::Agile )
{
}

AIArchetype SpawnerGroup::GetEnemyType(void) const
{
    return m_enemyType;
}

void SpawnerGroup::SetEnemyType(AIArchetype enemyType)
{
    m_enemyType = enemyType;
}

#if defined(URSINE_WITH_EDITOR)

void SpawnerGroup::createSpawner(void)
{
    // Check to see if there are spawners that exist for this enemy type
    // If so send a notification

    // Create a spawner object for the given enemy type
    // Store it in our map


    // IN SPAWN GROUP SYSTEM:
    // - Listen for Component removed "Spawner".  If removed, make sure to
    // remove it from it's coresponding spawnerGroupComponent.
    // - In OnAfterWorldLoad check for all entites called spawnerGroup, and
    // add all their children to their map.

    // IN ON REMOVE IN THIS COMPONENT:
    // Delete all children.
}

#endif
