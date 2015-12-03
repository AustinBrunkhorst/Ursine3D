#pragma once

#include <EntitySystem.h>
#include "SpawnpointComponent.h"

class SpawnSystem : public ursine::ecs::EntitySystem
{
    ENTITY_SYSTEM;

public:
    SpawnSystem(ursine::ecs::World *world);

private:

    void OnInitialize(void) override;
    void OnRemove(void) override;

    // used to maintain player count and spawnpoint list
    void onComponentAdded(EVENT_HANDLER(World));
    // spawn points and player count
    void onComponentRemoved(EVENT_HANDLER(World));

    //Function for getting list of spawn points and creating player archtypes at those positions
    void spawnPlayer(int team);

    const ursine::SVec3 &getSpawnPosition(int);

    //std::vector<Spawnpoint *> m_spawnpoints;

    std::vector<std::list<Spawnpoint *>> m_spawnpointLists;

    unsigned m_playerCount;
    unsigned m_maxPlayerCount;

} Meta(Disable);