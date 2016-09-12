/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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
#include "SpawnerComponent.h"

#include "LevelSegmentManagerComponent.h"

#include <Notification.h>
#include <SystemManager.h>

NATIVE_COMPONENT_DEFINITION( SpawnerGroup );

using namespace ursine;
using namespace ecs;

SpawnerGroup::SpawnerGroup(void)
    : BaseComponent( )
    , m_enemyType( AIArchetype::Agile )
    , m_activeEnemies( 0 )
{
}

SpawnerGroup::~SpawnerGroup(void)
{
    auto levelManager = GetOwner( )->GetWorld( )
        ->GetEntitiesFromFilter( Filter( ).All<LevelSegmentManager>( ) );

    if (levelManager.size( ))
        levelManager[ 0 ]->GetComponent<LevelSegmentManager>( )->Listener( this )
            .Off( LevelSegmentManagerEvents::SegmentChanged, &SpawnerGroup::onLevelSegmentChange );
}

void SpawnerGroup::OnInitialize(void)
{
    auto levelManager = GetOwner( )->GetWorld( )
        ->GetEntitiesFromFilter( Filter( ).All<LevelSegmentManager>( ) );

    if (levelManager.size( ))
        levelManager[ 0 ]->GetComponent<LevelSegmentManager>( )->Listener( this )
            .On( LevelSegmentManagerEvents::SegmentChanged, &SpawnerGroup::onLevelSegmentChange );
}

AIArchetype SpawnerGroup::GetEnemyType(void) const
{
    return m_enemyType;
}

void SpawnerGroup::SetEnemyType(AIArchetype enemyType)
{
    m_enemyType = enemyType;
}

int SpawnerGroup::GetActiveEnemiesCount(void) const
{
    return m_activeEnemies;
}

void SpawnerGroup::addSpawner(Spawner *spawner)
{
    m_spawners[ spawner->m_enemyType ] = spawner;
}

void SpawnerGroup::removeSpawner(Spawner *spawner)
{
    UAssert( haveSpawnerOfType( spawner->m_enemyType ), "Error: Why is this happening?" );

    m_spawners.erase( spawner->m_enemyType );
}

bool SpawnerGroup::haveSpawnerOfType(AIArchetype type)
{
    return m_spawners.find( type ) != m_spawners.end( );
}

void SpawnerGroup::update(void)
{
    // iterate through all spawners and update them
    for (auto &spawnerPair : m_spawners)
    {
        spawnerPair.second->update( this );
    }
}

void SpawnerGroup::onLevelSegmentChange(EVENT_HANDLER(LevelSegmentManager))
{
    EVENT_ATTRS(LevelSegmentManager, LevelSegmentChangeArgs);

    // notify the spawners that the segment changed
    for (auto &spawnerPair : m_spawners)
    {
        spawnerPair.second->onLevelSegmentChange( args->segment );
    }
}

#if defined(URSINE_WITH_EDITOR)

void SpawnerGroup::createSpawner(void)
{
    // Check to see if there are spawners that exist for this enemy type
    // If so send a notification
    if (haveSpawnerOfType( m_enemyType ))
    {
        NotificationConfig config;

        config.type = NOTIFY_INFO;
        config.header = "Notice";
        config.message = "This group already has a spawner of that enemy type.";
        config.dismissible = true;
        config.duration = TimeSpan::FromSeconds( 5.0f );

        EditorPostNotification( config );

        return;
    }

    // Create a spawner object for the given enemy type
    // Store it in our map
    auto entity = GetOwner( )->GetWorld( )->CreateEntity( );

    // set the name
    switch (m_enemyType)
    {
        case AIArchetype::Agile:
        {
            entity->SetName( "Spawner_Agile" );
            break;
        }
        case AIArchetype::Bomber:
        {
            entity->SetName( "Spawner_Bomber" );
            break;
        }
        case AIArchetype::Fodder:
        {
            entity->SetName( "Spawner_Fodder" );
            break;
        }
        case AIArchetype::Nuker:
        {
            entity->SetName( "Spawner_Nuker" );
            break;
        }
        case AIArchetype::Tank:
        {
            entity->SetName( "Spawner_Tank" );
            break;
        }
    }

    auto spawner = entity->AddComponent<Spawner>( );

    spawner->m_enemyType = m_enemyType;

    addSpawner( spawner );

    auto entityTrans = entity->GetTransform( );

    entityTrans->SetWorldPosition( SVec3::Zero( ) );

    GetOwner( )->GetTransform( )->AddChildAlreadyInLocal( entityTrans );
}

#endif
