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
#include <SystemManager.h>

using namespace ursine;


ENTITY_SYSTEM_DEFINITION( SpawnSystem );

SpawnSystem::SpawnSystem(ursine::ecs::World* world) : EntitySystem(world)
{
}

void SpawnSystem::DespawnTeam(int team)
{
    if (team == 1)
    {
        while (m_team1.size() > 0)
        {
            auto *player = m_team1.front();
            m_team1.pop_front();

            player->GetOwner()->Delete();
        }
    }
    else if (team == 2)
    {
        while (m_team2.size() > 0)
        {
            auto *player = m_team2.front();
            m_team2.pop_front();

            player->GetOwner()->Delete();
        }
    }
}

struct doCompare
{
    doCompare()
    { }

    bool operator()(Spawnpoint *first, Spawnpoint *second)
    {
        return first->roundSpawnNnumber < second->roundSpawnNnumber;
    }
};


void SpawnSystem::OnInitialize(void)
{
    m_world->Listener(this)
        .On(ecs::WorldEventType::WORLD_ENTITY_COMPONENT_ADDED, &SpawnSystem::onComponentAdded)
        .On(ecs::WorldEventType::WORLD_ENTITY_COMPONENT_REMOVED, &SpawnSystem::onComponentRemoved);

    m_world->GetEntitySystem(RoundSystem)->Listener(this)
        .On(ROUND_START, &SpawnSystem::onRoundStart);

    auto spawns = m_world->GetEntitiesFromFilter(ecs::Filter( ).All<Spawnpoint>());

    for (auto spawn : spawns)
    {
        auto *spawnComp = spawn->GetComponent<Spawnpoint>();

        if (spawnComp->teamNumber == 1)
        {
            m_team1Spawnpoints.push_back(spawnComp);
        }
        else if (spawnComp->teamNumber == 2)
        {
            m_team2Spawnpoints.push_back( spawnComp );
        }
    }

    //nameList.sort( [] (const name &a, const name &b)->bool{return a.lastName < b.lastName; });

    doCompare compfn;
    
    m_team1Spawnpoints.sort(compfn);
    m_team2Spawnpoints.sort(compfn);

}

void SpawnSystem::OnRemove(void)
{
    m_world->Listener(this)
        .Off(ecs::WorldEventType::WORLD_ENTITY_COMPONENT_ADDED, &SpawnSystem::onComponentAdded)
        .Off(ecs::WorldEventType::WORLD_ENTITY_COMPONENT_REMOVED, &SpawnSystem::onComponentRemoved);

    m_world->GetEntitySystem(RoundSystem)->Listener(this)
        .Off(ROUND_START, &SpawnSystem::onRoundStart);
}

void SpawnSystem::onComponentAdded(EVENT_HANDLER(ursine::ecs:::World))
{
    EVENT_ATTRS(ecs::World, ecs::ComponentEventArgs);

    if (args->component->Is<TeamComponent>())
    {
        auto teamComp = reinterpret_cast<TeamComponent *>(args->component);

        int team = teamComp->TeamNumber;

        if (team == 1)
        {
            m_team1.push_front(teamComp);
        }
        else if (team == 2)
        {
            m_team2.push_front(teamComp);
        }

    }
    else if (args->component->Is<Spawnpoint>())
    {
        auto *spawnpoint = reinterpret_cast<Spawnpoint *>(args->component);
        int team = spawnpoint->teamNumber;

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


    if (args->component->Is<TeamComponent>())
    {        
        TeamComponent *player = reinterpret_cast<TeamComponent *>(args->component);

        if (player->TeamNumber == 1)
        {
            m_team1.remove(player);

            if (m_team1.size() == 0)
            {
                RoundSystem::RoundEventArgs e(1);

                Dispatch(ROUND_OVER, &e);
            }
        }
        else if(player->TeamNumber == 2)
        {
            m_team2.remove(player);

            if (m_team2.size() == 0)
            {
                RoundSystem::RoundEventArgs e(2);

                Dispatch(ROUND_OVER, &e);
            }
        }
        
    }
    else if (args->component->Is<Spawnpoint>())
    {
        auto spawnpoint = reinterpret_cast<Spawnpoint *>(args->component);

        int team = spawnpoint->teamNumber;

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

    for (int i = 1; i <= args->team; ++i)
    {
        SpawnPlayer(1, i);
        SpawnPlayer(2, i);
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
    playerTransform->SetWorldPosition(getSpawnPosition(team, roundNum, 10.0f));
}

ursine::SVec3 SpawnSystem::getSpawnPosition(int team, int roundNum, float yOffset)
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
    return spawnPos + SVec3(0.0f, yOffset, 0.0f);
}