/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SpawnerComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - CJ Payne
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "SpawnerComponent.h"
#include "SpawnPatternContainerComponent.h"
#include "SpawnerGroupComponent.h"

NATIVE_COMPONENT_DEFINITION( Spawner );

using namespace ursine;
using namespace ecs;

Spawner::Spawner(void)
    : BaseComponent( )
{
}

AIArchetype Spawner::GetEnemyType(void) const
{
    return m_enemyType;
}

void Spawner::update(SpawnerGroup* group)
{
    // iterate through all patterns and update them
    for (auto &patterns : m_activePatterns)
    {
        // If this pattern's level event is equal to the current level's event
        patterns->update( group, this );
    }
}

void Spawner::onLevelSegmentChange(LevelSegments segment)
{
    // remove all patterns from the active vector, since they will no
    // longer be updating
    m_activePatterns.clear( );

    // iterate through all patterns and check to see if they're level
    // event is matching the new level event we were passed in
    for (auto &patterns : GetOwner( )->GetComponentsInChildren<SpawnPatternContainer>( ))
    {
        if (patterns->levelSegment == segment)
            m_activePatterns.push_back( patterns );
    }
}

Entity *Spawner::spawnEnemy(SpawnerGroup *group, const SVec3 &worldPosition)
{
    std::string archetypeName;

    switch (m_enemyType)
    {
        case AIArchetype::Fodder:
        {
            archetypeName = "Fodder";
            break;
        }
        case AIArchetype::Agile:
        {
            archetypeName = "Agile";
            break;
        }
        case AIArchetype::Bomber:
        {
            archetypeName = "Bomber";
            break;
        }
        case AIArchetype::Tank:
        {
            archetypeName = "Tank";
            break;
        }
        case AIArchetype::Nuker:
        {
            archetypeName = "Nuker";
            break;
        }
    }

    auto world = GetOwner( )->GetWorld( );

    auto entity = world->CreateEntityFromArchetype(
        WORLD_ARCHETYPE_PATH + archetypeName + ".uatype", archetypeName 
    );

    entity->GetTransform( )->SetWorldPosition( worldPosition );

    // Subscribe to it's dead event so we can keep a counter
    // Let the group know that a new enemy was spawned

    return entity;
}

#if defined(URSINE_WITH_EDITOR)

void Spawner::createPattern(void)
{
    auto owner = GetOwner( );
    auto newPattern = owner->GetWorld( )->CreateEntity( "NewPattern" );

    owner->GetTransform( )->AddChildAlreadyInLocal( newPattern->GetTransform( ) );

    newPattern->AddComponent<SpawnPatternContainer>( );

    newPattern->EnableHierarchyChange( false );
}

#endif
