#include "Precompiled.h"

#include <random>
#include <algorithm>
#include "SpawnSystem.h"
#include "RoundSystem.h"

#include <WorldEvent.h>

#include <SystemManager.h>
#include "CoreSystem.h"
#include <WorldEvent.h>
#include <EventDispatcher.h>
#include "RecorderSystem.h"
#include <PlayerInputComponent.h>
#include <Game Engine/Scene/Component/Native Components/CameraComponent.h>

using namespace ursine;


ENTITY_SYSTEM_DEFINITION( SpawnSystem );

SpawnSystem::SpawnSystem(ursine::ecs::World* world) 
	: EntitySystem(world)
	, m_teams({ std::vector<TeamComponent *>(), std::vector<TeamComponent *>() })
{
}

void SpawnSystem::DespawnTeam(int team)
{
	auto &teamVec = m_teams[ team - 1 ];

	for (auto &p : teamVec)
	{
		p->GetOwner( )->Delete( );
	}

	teamVec.clear( );
}

struct doCompare
{
    doCompare()
    { }

    bool operator()(Spawnpoint *first, Spawnpoint *second)
    {
        return first->GetRoundSpawnNumber() < second->GetRoundSpawnNumber();
    }
};


void SpawnSystem::OnInitialize(void)
{
    m_world->Listener(this)
        .On(ecs::WorldEventType::WORLD_ENTITY_COMPONENT_ADDED, &SpawnSystem::onComponentAdded)
        .On(ecs::WorldEventType::WORLD_ENTITY_COMPONENT_REMOVED, &SpawnSystem::onComponentRemoved);

    m_world->GetEntitySystem(RoundSystem)->Listener(this)
        .On(ROUND_START, &SpawnSystem::onRoundStart)
		.On(PLAYER_DIED, &SpawnSystem::onPlayerDied);

}

void SpawnSystem::OnRemove(void)
{
    m_world->Listener(this)
        .Off(ecs::WorldEventType::WORLD_ENTITY_COMPONENT_ADDED, &SpawnSystem::onComponentAdded)
        .Off(ecs::WorldEventType::WORLD_ENTITY_COMPONENT_REMOVED, &SpawnSystem::onComponentRemoved);

    m_world->GetEntitySystem(RoundSystem)->Listener(this)
        .Off(ROUND_START, &SpawnSystem::onRoundStart)
		.Off(PLAYER_DIED, &SpawnSystem::onPlayerDied);
}

void SpawnSystem::onComponentAdded(EVENT_HANDLER(ursine::ecs:::World))
{
    EVENT_ATTRS(ecs::World, ecs::ComponentEventArgs);

    if (args->component->Is<TeamComponent>())
    {
        auto teamComp = reinterpret_cast<TeamComponent *>(args->component);

        int team = teamComp->GetTeamNumber();

		m_teams[ team - 1 ].push_back( teamComp );
    }
    else if (args->component->Is<Spawnpoint>())
    {
        auto *spawnpoint = reinterpret_cast<Spawnpoint *>(args->component);
        int team = spawnpoint->GetTeamNumber();

        if (team == 1)
        {
            m_team1Spawnpoints.push_back(spawnpoint);
        }
        else if (team == 2)
        {
            m_team2Spawnpoints.push_back(spawnpoint);

        }

        doCompare compfn;

        // this is to nicely different teams spawnpoints
        m_team1Spawnpoints.sort(compfn);
        m_team2Spawnpoints.sort(compfn);
    }
}

void SpawnSystem::onComponentRemoved(EVENT_HANDLER(ursine::ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::ComponentEventArgs);

    if (args->component->Is<TeamComponent>( ))
	{
        TeamComponent *player = reinterpret_cast<TeamComponent *>(args->component);
        //????
    }
    else if (args->component->Is<Spawnpoint>())
    {
        auto spawnpoint = reinterpret_cast<Spawnpoint *>(args->component);

        int team = spawnpoint->GetTeamNumber();

        if (team == 1)
        {
            m_team1Spawnpoints.remove(spawnpoint);
        }
        else if (team == 2)
        {
            m_team2Spawnpoints.remove(spawnpoint);
        }
    }
}

void SpawnSystem::onRoundStart(EVENT_HANDLER(RoundSystem))
{
    EVENT_ATTRS(RoundSystem, RoundSystem::RoundEventArgs);
	
	// Iterate through current team players, setting them to alive
	for (auto playerList : m_teams)
	{
		int roundNum = 0;
		for (auto player : playerList)
		{
			// Set health to 100
			player->SetAlive();

			// Set spawn point
			player->GetOwner( )->GetTransform( )->GetRoot( )->SetWorldPosition(
				getSpawnPosition( player->GetTeamNumber( ), ++roundNum )
			);
		}
	}

	// Add a new player to each team for this current round (this is our current player entity that is the player)
	SpawnPlayer( 1, args->team );
	SpawnPlayer( 2, args->team );

	// Tell the recorder system that the round has started
	auto recorder = m_world->GetEntitySystem( RecorderSystem );

	recorder->SetRoundStart( m_teams );
}

void SpawnSystem::onPlayerDied(EVENT_HANDLER(RoundSystem))
{
	EVENT_ATTRS(RoundSystem, RoundSystem::RoundEventArgs);

	// set that player to dead
	args->entity->GetComponent<TeamComponent>()->SetDead();

	// remove the input component
	args->entity->RemoveComponent<PlayerInput>( );

	// play an animation

	// see if the round ended due to whole team killed
	auto team = args->entity->GetComponent<TeamComponent>( );

	auto &teamVec = m_teams[ team->GetTeamNumber( ) - 1 ];

    args->entity->GetComponentInChildren<ecs::Camera>()
        ->GetOwner()->RemoveComponent<ecs::Camera>();

	bool allDead = true;
	for (auto &p : teamVec)
	{
		if (!p->IsDead( ))
		{
			allDead = false;
			break;
		}
	}

	if (allDead)
	{
		m_world->GetSystemManager()->GetSystem<RoundSystem>()
			->StartNewRound(team->GetTeamNumber());
	}
}

void SpawnSystem::SpawnPlayer(int team, int roundNum)
{
    ecs::Entity *newPlayer = nullptr;
    if (team == 1)
    {
        newPlayer = m_world->CreateEntityFromArchetype(
            WORLD_ARCHETYPE_PATH "Player1.uatype",
            "SpawnedPlayerTeam1"
            );
    }
    else
    {
        newPlayer = m_world->CreateEntityFromArchetype(
            WORLD_ARCHETYPE_PATH "Player2.uatype",
            "SpawnedPlayerTeam2"
            );
    }

    auto *playerTransform = newPlayer->GetTransform();

    // get spawn position to place the player at and actually spawn the player
    playerTransform->SetWorldPosition(getSpawnPosition(team, roundNum));
}

ursine::SVec3 SpawnSystem::getSpawnPosition(int team, int roundNum)
{

    const std::list<Spawnpoint *>& spawnList = (team == 1) ? m_team1Spawnpoints : m_team2Spawnpoints;


    Spawnpoint *chosenSpawn = nullptr;

    if (roundNum > spawnList.size())
    {
        return SVec3( 0.0f, 10.0f, 0.0f );
    }

    int index = roundNum - 1;

    int count = 0;
    for (auto i = spawnList.begin(); i != spawnList.end(); ++i, ++count)
    {
        if (count == index)
        {
            chosenSpawn = *i;
        }
    }
    auto spawnPos = chosenSpawn->GetOwner()->GetTransform()->GetWorldPosition();
    return spawnPos;
}