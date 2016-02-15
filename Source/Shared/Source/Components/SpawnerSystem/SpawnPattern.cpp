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

SpawnPattern::SpawnPattern(void)
	: m_active( true )
	, m_totalEnemies( 1 )
	, m_maxNumberOfEnemies( 1 )
	, m_minNumberOfEnemies( 1 )
	, m_singleSpawnCooldown( 1.0f )
	, m_triggerNextPattern( false )
	, m_loopPattern( true )
	, m_spawnDuration( 5.0f )
	, m_spawnerBreakTime( 1.0f )
	, m_spawnerDistanceVariance( 5.0f )
	, m_spawnDirection( 1.0f, 0.0f, 1.0f )
	, m_spawnAngleVariance( 180.0f )
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

void SpawnPattern::SetTotalEnemies(bool total)
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
	return m_spawnerDistanceVariance;
}

void SpawnPattern::SetSpawnDistanceVariance(float variance)
{
	m_spawnerDistanceVariance = variance;
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
