/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SpawnPatter.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - CJ Payne
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "SpawnPattern.h"

#include "SpawnerGroupComponent.h"
#include "SpawnerComponent.h"
#include "SpawnPatternContainerComponent.h"

#include "GameEvents.h"

#include <Application.h>

using namespace ursine;
using namespace ecs;

SpawnPattern::SpawnPattern(void)
    : m_active( true )
    , m_triggerNextPattern(true)
    , m_loopPattern(true)
    , m_endingSegmentTransition( LevelSegments::Empty )
    , m_totalEnemies( 1 )
    , m_maxNumberOfEnemies( 1 )
    , m_minNumberOfEnemies( 1 )
    , m_singleSpawnCooldown( 1.0f )
    , m_spawnDuration( 5.0f )
    , m_spawnerBreakTime( 1.0f )
    , m_spawnDistanceVariance( 5.0f )
    , m_spawnDirection( 1.0f, 0.0f, 1.0f )
    , m_spawnAngleVariance( 180.0f )
    , m_spawnTimer( 0.0f )
    , m_activeEnemies( 0 )
    , m_totalEnemiesSpawned( 0 )
    , m_breakTimer( 0.0f )
    , m_breaking( false )
{
}

bool SpawnPattern::GetActive(void) const
{
    return m_active;
}

void SpawnPattern::SetActive(bool active)
{
    m_active = active;
}

int SpawnPattern::GetTotalEnemies(void) const
{
    return m_totalEnemies;
}

void SpawnPattern::SetTotalEnemies(int total)
{
    m_totalEnemies = total;
}

int SpawnPattern::GetMaxNumberOfEnemies(void) const
{
    return m_maxNumberOfEnemies;
}

void SpawnPattern::SetMaxNumberOfEnemies(int max)
{
    m_maxNumberOfEnemies = max;
}

int SpawnPattern::GetMinNumberOfEnemies(void) const
{
    return m_minNumberOfEnemies;
}

void SpawnPattern::SetMinNumberOfEnemies(int min)
{
    m_minNumberOfEnemies = min;
}

float SpawnPattern::GetSingleSpawnCooldown(void) const
{
    return m_singleSpawnCooldown;
}

void SpawnPattern::SetSingleSpawnCooldown(float cooldown)
{
    m_singleSpawnCooldown = cooldown;
}

bool SpawnPattern::GetTriggerNextPattern(void) const
{
    return m_triggerNextPattern;
}

void SpawnPattern::SetTriggerNextPattern(bool flag)
{
    m_triggerNextPattern = flag;
}

bool SpawnPattern::GetLoopPattern(void) const
{
    return m_loopPattern;
}

void SpawnPattern::SetLoopPattern(bool loop)
{
    m_loopPattern = loop;
}

LevelSegments SpawnPattern::GetEndingSegmentTransition(void) const
{
    return m_endingSegmentTransition;
}

void SpawnPattern::SetEndingSegmentTransition(LevelSegments segment)
{
    m_endingSegmentTransition = segment;
}

float SpawnPattern::GetSpawnDuration(void) const
{
    return m_spawnDuration;
}

void SpawnPattern::SetSpawnDuration(float duration)
{
    m_spawnDuration = duration;
}

float SpawnPattern::GetSpawnerBreakTime(void) const
{
    return m_spawnerBreakTime;
}

void SpawnPattern::SetSpawnerBreakTime(float breakTime)
{
    m_spawnerBreakTime = breakTime;
}

float SpawnPattern::GetSpawnDistanceVariance(void) const
{
    return m_spawnDistanceVariance;
}

void SpawnPattern::SetSpawnDistanceVariance(float variance)
{
    m_spawnDistanceVariance = variance;
}

const ursine::SVec3 &SpawnPattern::GetSpawnDirection(void) const
{
    return m_spawnDirection;
}

void SpawnPattern::SetSpawnDirection(const ursine::SVec3 &direction)
{
    m_spawnDirection = direction;
}

float SpawnPattern::GetSpawnAngleVariance(void) const
{
    return m_spawnAngleVariance;
}

void SpawnPattern::SetSpawnAngleVariance(float variance)
{
    m_spawnAngleVariance = variance;
}

void SpawnPattern::Update(SpawnerGroup *group, Spawner *spawner, SpawnPatternContainer *container)
{
    auto dt = Application::Instance->GetDeltaTime( );
    
    // If we've spawned the total amount of enemies
    if (m_totalEnemiesSpawned >= m_totalEnemies)
    {
        // Early out if we aren't going to loop.  This means that
        // we have already spawned all our enemies, and are going
        // to let the duration finish out.
        if (!m_loopPattern)
            return;

        // If we're going to loop, check to see if we can reset
        if (m_activeEnemies == 0)
        {
            // if we aren't breaking, start the break
            if (!m_breaking)
            {
                m_breakTimer = m_spawnerBreakTime;
                m_breaking = true;
            }
            // If we are breaking, decrement timer
            else
            {
                m_breakTimer -= dt;

                // If timer is up, start to spawn again
                if (m_breakTimer <= 0.0f)
                {
                    m_breaking = false;
                    m_totalEnemiesSpawned = 0;
                    m_spawnTimer = 0.0f;
                }
            }
        }
    }
    // Else, we're still in the process of spawning
    else if (m_activeEnemies < m_maxNumberOfEnemies)
    {
        // Get back up to our quota
        if (m_activeEnemies < m_minNumberOfEnemies)
        {
            while (m_activeEnemies < m_minNumberOfEnemies && m_totalEnemiesSpawned < m_totalEnemies)
            {
                // Spawn an enemy
                spawn( spawner, container );
            }

            // Reset the spawn timer
            m_spawnTimer = 0.0f;
        }
        // Else, wait to spawn some more
        else
        {
            // Increment the spawn timer
            m_spawnTimer += dt;

            // Check to see if we should spawn something
            if (m_spawnTimer > m_singleSpawnCooldown)
            {
                spawn( spawner, container );
            }
        }
    }

}

void SpawnPattern::spawn(Spawner *spawner, SpawnPatternContainer *container)
{
    // Get the origin
    auto origin = container->GetOwner( )->GetTransform( )->GetWorldPosition( );
    
    // Calculate the offset
    auto perp = SVec3::Cross( 
        m_spawnDirection,
        m_spawnDirection == SVec3::UnitY( ) ? SVec3::UnitX( ) : SVec3::UnitY( ) 
    );

    auto normal = SVec3::Cross( perp, m_spawnDirection );

    SQuat rotation( math::Rand( -m_spawnAngleVariance, m_spawnAngleVariance ), normal );

    auto newDirection = rotation.Rotate( m_spawnDirection );

    auto offset = newDirection * math::Rand( 0.0f, m_spawnDistanceVariance );

    auto *entity = spawner->spawnEnemy( origin + offset );

    ++m_activeEnemies;
    ++m_totalEnemiesSpawned;

    entity->Listener( this )
        .On( game::ENEMY_DEATH, &SpawnPattern::onEnemyDeath );
}

void SpawnPattern::onEnemyDeath(EVENT_HANDLER(Entity))
{
    --m_activeEnemies;
}