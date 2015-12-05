#pragma once

#include <EntitySystem.h>
#include "SpawnpointComponent.h"
#include <Components/TeamComponent.h>
#include "RoundSystem.h"


class SpawnSystem : public ursine::ecs::EntitySystem
                  , public ursine::EventDispatcher<RoundSystemEventType>
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
            void onRoundStart(EVENT_HANDLER(ursine::ecs::world));

            ursine::SVec3 getSpawnPosition(int team, int roundNum, float yOffset);

            //std::vector<Spawnpoint *> m_spawnpoints;
            std::list<TeamComponent *> m_team1;
            std::list<TeamComponent *> m_team2;

            std::list<Spawnpoint *> m_team1Spawnpoints;
            std::list<Spawnpoint *> m_team2Spawnpoints;

        } Meta(Enable);
