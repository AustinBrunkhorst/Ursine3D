#include "Precompiled.h"

#include <random>

#include "SpawnSystem.h"
#include <Components/CharacterControllerComponent.h>
#include <Components/PlayerInputComponent.h>


using namespace ursine;
using namespace ursine::ecs;

ENTITY_SYSTEM_DEFINITION( SpawnSystem );

SpawnSystem::SpawnSystem(ursine::ecs::World* world) : EntitySystem(world)
                                                    , m_playerCount(0)
                                                    , m_maxPlayerCount(0)
{
}

void SpawnSystem::OnInitialize(void)
{
    m_world->Listener(this)
        .On(WORLD_ENTITY_COMPONENT_ADDED, &SpawnSystem::onComponentAdded)
        .On(WORLD_ENTITY_COMPONENT_REMOVED, &SpawnSystem::onComponentRemoved);
}

void SpawnSystem::OnRemove(void)
{
    m_world->Listener(this)
        .Off(WORLD_ENTITY_COMPONENT_ADDED, &SpawnSystem::onComponentAdded)
        .Off(WORLD_ENTITY_COMPONENT_REMOVED, &SpawnSystem::onComponentRemoved);
}

void SpawnSystem::onComponentAdded(EVENT_HANDLER(World))
{
    EVENT_ATTRS(World, ComponentEventArgs);

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
        while (spawnpoint->teamNumber >= m_spawnpointLists.size())
        {
            m_spawnpointLists.push_back(std::list<Spawnpoint *>());
        }
        // this is to nicely different teams spawnpoints
        m_spawnpointLists[spawnpoint->teamNumber].push_back(spawnpoint);        
    }
}

void SpawnSystem::onComponentRemoved(EVENT_HANDLER(World))
{
    EVENT_ATTRS(World, ComponentEventArgs);


    if (args->component->Is<CharacterController>())
    {
        --m_playerCount;

        if (m_playerCount == 0)
        {
            for (unsigned i = 0; i < m_maxPlayerCount / 2; ++i)
            {
                spawnPlayer(1);
                spawnPlayer(2);
            }
        }
    }
    else if (args->component->Is<Spawnpoint>())
    {
        auto spawnpoint = reinterpret_cast<Spawnpoint *>(args->component);

        m_spawnpointLists[spawnpoint->teamNumber].remove(spawnpoint);
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

    std::uniform_int_distribution<> randNo(0, m_spawnpointLists[team].size() - 1);

    int index = randNo(gen);

    while (m_spawnpointLists[team][index]->teamNumber != team)
    {
        index = randNo(gen);
    }
}