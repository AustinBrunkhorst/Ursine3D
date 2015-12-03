#include "Precompiled.h"

#include <random>

#include "SpawnSystem.h"
#include <Components/CharacterControllerComponent.h>
#include <Components/PlayerInputComponent.h>
#include <WorldEvent.h>

#include <GfxAPI.h> // WHO IS YOUR GOD


#include <SystemManager.h>
#include "CoreSystem.h"
#include <WorldEvent.h>

using namespace ursine;


ENTITY_SYSTEM_DEFINITION( SpawnSystem );

SpawnSystem::SpawnSystem(ursine::ecs::World* world) : EntitySystem(world)
                                                    , m_playerCount(0)
                                                    , m_maxPlayerCount(0)
{
}

void SpawnSystem::OnInitialize(void)
{
    m_world->Listener(this)
        .On(ecs::WorldEventType::WORLD_ENTITY_COMPONENT_ADDED, &SpawnSystem::onComponentAdded)
        .On(ecs::WorldEventType::WORLD_ENTITY_COMPONENT_REMOVED, &SpawnSystem::onComponentRemoved);
}

void SpawnSystem::OnRemove(void)
{
    //m_world->Listener(this)
    //    .Off(WORLD_ENTITY_COMPONENT_ADDED, &SpawnSystem::onComponentAdded)
    //    .Off(WORLD_ENTITY_COMPONENT_REMOVED, &SpawnSystem::onComponentRemoved);
}

void SpawnSystem::onComponentAdded(EVENT_HANDLER(ursine::ecs:::World))
{
    EVENT_ATTRS(ecs::World, ecs::ComponentEventArgs);

    if (args->component->Is<CharacterController>())
    {
        ++m_playerCount;

        if (m_playerCount > m_maxPlayerCount)
        {
            m_maxPlayerCount = m_playerCount;
        }
    }
    else if (args->component->Is<Spawnpoint>())
    {
        auto *spawnpoint = reinterpret_cast<Spawnpoint *>(args->component);
        int team = spawnpoint->teamNumber;

        if (team == 0)
        {
            m_team1Spawnpoints.push_back(spawnpoint);
        }
        else if (team == 1)
        {
            m_team2Spawnpoints.push_back(spawnpoint);

        }

        // this is to nicely different teams spawnpoints

    }
}

void SpawnSystem::onComponentRemoved(EVENT_HANDLER(ursine::ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::ComponentEventArgs);


    if (args->component->Is<CharacterController>())
    {
        --m_playerCount;

        if (m_playerCount == 0)
        {
            for (unsigned i = 0; i < m_maxPlayerCount / 2; ++i)
            {
                spawnPlayer(0);
                spawnPlayer(1);
            }
        }
    }
    else if (args->component->Is<Spawnpoint>())
    {
        auto spawnpoint = reinterpret_cast<Spawnpoint *>(args->component);

        int team = spawnpoint->teamNumber;

        if (team == 0)
        {
            m_team1Spawnpoints.remove(spawnpoint);
        }
        else if (team == 1)
        {
            m_team2Spawnpoints.remove(spawnpoint);
        }
    }
}

void SpawnSystem::spawnPlayer(int team)
{
    auto *newPlayer = m_world->CreateEntityFromArchetype("Player", "SpawnedPlayer");

    auto *playerTransform = newPlayer->GetTransform();

    auto *playerInput = newPlayer->GetComponent<PlayerInput>();

    playerInput->id = team;

    // get spawn position to place the player at and actually spawn the player
    playerTransform->SetWorldPosition(getSpawnPosition(team));
}

const ursine::SVec3& SpawnSystem::getSpawnPosition(int team)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    const std::list<Spawnpoint *>& spawnList = (team == 1) ? m_team1Spawnpoints : m_team2Spawnpoints;


    std::uniform_int_distribution<> randNo(0, static_cast<int>(spawnList.size()) - 1);

    Spawnpoint *chosenSpawn = nullptr;

    int index = randNo(gen);

    int count = 0;
    for (auto i = spawnList.begin(); i != spawnList.end(); ++i, ++count)
    {
        if (count == index)
        {
            chosenSpawn = *i;
        }
    }

    return chosenSpawn->GetOwner()->GetTransform()->GetWorldPosition();
}