/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SpawnSystem.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <EntitySystem.h>
#include "SpawnpointComponent.h"
#include <Components/TeamComponent.h>
#include "RoundSystem.h"


class SpawnSystem : public ursine::ecs::EntitySystem
{
    ENTITY_SYSTEM;

public:
    SpawnSystem(ursine::ecs::World *world);

    void DespawnTeam(int team);

    void SpawnPlayer(int team, int roundNum);

private:
    void OnInitialize(void) override;
    void OnRemove(void) override;

    // used to maintain player count and spawnpoint list
    void onComponentAdded(EVENT_HANDLER(ursine::ecs:::World));

    // spawn points and player count
    void onComponentRemoved(EVENT_HANDLER(ursine::ecs::World));

    // spawn players when the round starts based off of round number
    void onRoundStart(EVENT_HANDLER(RoundSystem));

	// when the players die, this gets called
	void onPlayerDied(EVENT_HANDLER(RoundSystem));

    void killPlayer(ursine::ecs::Entity *entity);

    Spawnpoint* getSpawner(int team, int roundNum);

    std::vector<std::vector<TeamComponent *> > m_teams;

    std::list<Spawnpoint *> m_team1Spawnpoints;
    std::list<Spawnpoint *> m_team2Spawnpoints;

} Meta(Enable, DisableEntitySystemAutoAdd);
