/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossSpawnVinesState.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "BossSpawnVinesState.h"

#include "BossAIStateMachine.h"
#include "BossAIComponent.h"

#include "VineSpawnerComponent.h"
#include "Phase3BossSpawnComponent.h"

#include <World.h>
#include <Application.h>

using namespace ursine;
using namespace ecs;

BossSpawnVinesState::BossSpawnVinesState(LevelSegments spawnSegment, float delay)
    : BossAIState( "Spawn Vines" )
    , m_spawnSegment( spawnSegment )
    , m_delay( delay )
    , m_delayTimer( 0.0f )
    , m_finished( false )
    , m_index( 0 )
{
}

void BossSpawnVinesState::Enter(BossAIStateMachine *machine)
{
    auto boss = machine->GetBoss( );
    auto world = boss->GetOwner( )->GetWorld( );

    auto spawners = world->GetEntitiesFromFilter( Filter( ).All<VineSpawner>( ) );

    for (auto &spawner : spawners)
    {
        if (spawner->GetComponent<VineSpawner>( )->GetSpawnSegment( ) != m_spawnSegment ||
            spawner->HasComponent<Phase3BossSpawn>( ))
            continue;

        m_spawners.push_back( spawner );
    }

    if (m_spawners.size( ) == 0)
        m_finished = true;
    else
        m_finished = false;
}

void BossSpawnVinesState::Update(BossAIStateMachine *machine)
{
    if (m_finished)
        return;

    float dt = Application::Instance->GetDeltaTime( );

    m_delayTimer -= dt;

    // Check to see if we can spawn now
    if (m_delayTimer <= 0.0f)
    {
        auto boss = machine->GetBoss( );
        auto world = boss->GetOwner( )->GetWorld( );
        auto vineArchetype = boss->GetVineArchetype( );
        auto &spawner = m_spawners[ m_index ];

        // If the spawner is still a thing
        if (spawner)
        {
            // Get the transform
            auto spawnTrans = spawner->GetTransform( );

            // Create the vine entity
            auto vine = world->CreateEntityFromArchetype( vineArchetype );

            if (vine)
            {
                auto trans = vine->GetTransform( );

                trans->SetWorldPosition( spawnTrans->GetWorldPosition( ) );
                trans->SetWorldRotation( spawnTrans->GetWorldRotation( ) );

                boss->AddSpawnedVine( vine );
            }

            // Reset the timer
            m_delayTimer = m_delay;
        }

        // Increment the index
        ++m_index;

        // Check to see if we're done
        if (m_index >= m_spawners.size( ))
            m_finished = true;
    }
}

void BossSpawnVinesState::Exit(BossAIStateMachine *machine)
{
    m_index = 0;
    m_spawners.clear( );
    m_delayTimer = 0.0f;
}
