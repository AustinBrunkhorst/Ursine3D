/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SpawnPatterContainerComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - CJ Payne
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "SpawnPatternContainerComponent.h"
#include "LevelSegmentManagerComponent.h"

#include <Application.h>
#include <SystemManager.h>

NATIVE_COMPONENT_DEFINITION(SpawnPatternContainer);

using namespace ursine;
using namespace ecs;

SpawnPatternContainer::SpawnPatternContainer(void)
    : BaseComponent( )
    , levelSegment( LevelSegments::Empty )
    , m_index( 0 )
    , m_patternTimer( 0.0f )
    , m_breaking( false )
{
}

void SpawnPatternContainer::update(SpawnerGroup *group, Spawner *spawner)
{
    if (m_index >= spawnPatterns.Size( ))
        return;

    auto dt = Application::Instance->GetDeltaTime( );
    auto &pattern = spawnPatterns[ m_index ];

    // If the pattern isn't active, skip it
    if (!pattern.GetActive( ))
    {
        // Set the timer to the max so that we can enter the next if statement below
        m_patternTimer = pattern.GetSpawnDuration( ) + pattern.GetSpawnerBreakTime( );
    }
    // Else, update the pattern and let it spawn it's tings (and not breaking)
    else if (!m_breaking)
    {
        pattern.Update( group, spawner, this );
    }

    // Increment the timer and check to see if we're ready to move to the next pattern
    m_patternTimer += dt;

    if (!pattern.GetLoopPattern( ) && 
        (m_patternTimer >= pattern.GetSpawnDuration( ) ||
        (pattern.m_activeEnemies == 0 && pattern.m_totalEnemiesSpawned == pattern.m_totalEnemies)))
    {
        m_breaking = true;

        // If the current pattern sends an event, send it
        if (pattern.GetEndingSegmentTransition( ) != LevelSegments::Empty)
        {
            // Set the current segment
            auto levelManager = GetOwner( )->GetWorld( )
                ->GetEntitiesFromFilter( Filter( ).All<LevelSegmentManager>( ) );

            if (levelManager.size( ))
                levelManager[ 0 ]->GetComponent<LevelSegmentManager>( )
                    ->SetCurrentSegment( pattern.GetEndingSegmentTransition( ) );

            // we're not breaking anymore
            m_breaking = false;
        }

        // If we're breaking and we're done breaking
        if (m_breaking && m_patternTimer >= pattern.GetSpawnDuration( ) + pattern.GetSpawnerBreakTime( ))
        {
            // We're done breaking
            m_breaking = false;
        }

        // If we're done breaking, we should move to the next pattern
        if (!m_breaking)
        {
            if (pattern.GetTriggerNextPattern( ))
                ++m_index;
            else
                m_index = static_cast<int>( spawnPatterns.Size( ) );

            m_patternTimer = 0.0f;
        }
    }
}
