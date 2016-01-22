/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SpawnSystem.cpp
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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
#include <CameraComponent.h>
#include "CommandQueueComponent.h"
#include <Model3DComponent.h>
#include <Game Engine/Scene/Component/Native Components/ListenerComponent.h>
#include <RigidbodyComponent.h>
#include <PlayerAnimationComponent.h>
#include <Components/HealthComponent.h>

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

    if (args->component->Is<Spawnpoint>())
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

            player->GetOwner( )->GetComponent<Health>( )->SetHealth( 100.0f );

            // play an animation
            auto animator = player->GetOwner( )->GetComponentInChildren<PlayerAnimation>();

            animator->UnsetDead( );

            // Set the color
            auto teamColor = player->GetTeamNumber( ) == 1 ? Color::Blue : Color::Red;
            player->GetOwner( )->GetComponentInChildren<ecs::Model3D>( )->SetColor( teamColor );

			auto transform = player->GetOwner( )->GetTransform( );
			auto spawner = getSpawner( player->GetTeamNumber( ), ++roundNum );

			// Set spawn point
			transform->GetRoot( )->SetWorldPosition(
				spawner->GetOwner( )->GetTransform( )->GetWorldPosition( )
			);

			// Set spawn rotation
			transform->SetWorldRotation( SQuat( 0.0f, spawner->GetYRotationDegrees( ), 0.0f ) );

            // Add back the rigidbody
            if (!player->GetOwner()->HasComponent<ecs::Rigidbody>())
            {
                auto body = player->GetOwner()->AddComponent<ecs::Rigidbody>();

                body->SetBodyFlag(ecs::BodyFlag::Dynamic);
                body->SetMass(50.0f);
                body->SetGravity(SVec3(0.0f, -100.0f, 0.0f));
                body->SetRotationFreezeX(true);
                body->SetRotationFreezeY(true);
                body->SetRotationFreezeZ(true);
                body->SetSleepToggle(false);
                body->SetAwake();
            }
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
    
    killPlayer( args->entity );

    // play an animation
    auto animator = args->entity->GetComponentInChildren<PlayerAnimation>();

    animator->SetPlayerState(PlayerAnimation::Dead);


	// see if the round ended due to whole team killed
	auto team = args->entity->GetComponent<TeamComponent>( );
	auto &teamVec = m_teams[ team->GetTeamNumber( ) - 1 ];

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
        // 'Kill' the winning team's main player
        auto winningTeam = team->GetTeamNumber( ) == 1 ? 2 : 1;

        killPlayer( m_teams[ winningTeam - 1 ].back( )->GetOwner( ) );

		m_world->GetSystemManager( )->GetSystem<RoundSystem>( )
			->StartNewRound( team->GetTeamNumber( ) );
	}
}

void SpawnSystem::killPlayer(ursine::ecs::Entity* entity)
{
    // set that player to dead
	entity->GetComponent<TeamComponent>( )->SetDead( );

	// remove the input component
	entity->RemoveComponent<PlayerInput>( );

    // Set the command queue to stop recording
    auto commandQueue = entity->GetComponent<CommandQueue>( );
    commandQueue->SetRecording( false );
    commandQueue->UseRecorder( true );

    // Set the model's proper layer
    auto models = entity->GetComponentsInChildren<ecs::Model3D>( );
    for (auto &model : models)
    {
        if (model->GetOwner( )->GetName( ) == "FPSArm")
            model->SetRenderMask( 0 );
        else
            model->SetRenderMask( 4 );
    }

    // Set the color
    entity->GetComponentInChildren<ecs::Model3D>( )->SetColor( Color::Black );

    // Remove the camera
    auto cam = entity->GetComponentInChildren<ecs::Camera>( );

    if (cam)
        cam->GetOwner( )->RemoveComponent<ecs::Camera>( );

    // Remove the audio listener
    auto listener = entity->GetComponentInChildren<ecs::AudioListener>( );

    if (listener)
        listener->GetOwner( )->RemoveComponent<ecs::AudioListener>( );

    // Remove the rigidbody component (this is to fix an issue with the simulation)
    
    entity->RemoveComponent<ecs::Rigidbody>( );
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
    playerTransform->SetWorldPosition(
		getSpawner(team, roundNum)->GetOwner( )->GetTransform( )->GetWorldPosition( )
	);

    // Set him to record
    playerTransform->GetOwner( )->GetComponent<CommandQueue>( )->SetRecording( true );

    newPlayer->RemoveComponent<ecs::Rigidbody>( );

    // Add back the rigidbody
    auto body = newPlayer->AddComponent<ecs::Rigidbody>( );

    body->SetMass( 50.0f );
    body->SetGravity( SVec3( 0.0f, -100.0f, 0.0f ) );
    body->SetRotationFreezeX( true );
    body->SetRotationFreezeY( true );
    body->SetRotationFreezeZ( true );
    body->SetBodyFlag( ecs::BodyFlag::Dynamic );
    body->SetSleepToggle( false );
    body->SetAwake( );
}

Spawnpoint *SpawnSystem::getSpawner(int team, int roundNum)
{

    const std::list<Spawnpoint *>& spawnList = (team == 1) ? m_team1Spawnpoints : m_team2Spawnpoints;


    Spawnpoint *chosenSpawn = nullptr;

    if (roundNum > spawnList.size())
    {
        return nullptr;
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
    return chosenSpawn;
}