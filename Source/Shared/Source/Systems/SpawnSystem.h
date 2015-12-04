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
            void onComponentAdded(EVENT_HANDLER(ursine::ecs::World));
            // spawn points and player count
            void onComponentRemoved(EVENT_HANDLER(ursine::ecs::World));

            //Function for getting list of spawn points and creating player archtypes at those positions
            void spawnPlayer(int team);

            ursine::SVec3 getSpawnPosition(int team, float yOffset);

            //std::vector<Spawnpoint *> m_spawnpoints;

                std::list<Spawnpoint *> m_team1Spawnpoints;
                std::list<Spawnpoint *> m_team2Spawnpoints;

            unsigned m_playerCount;
            unsigned m_maxPlayerCount;

        } Meta(Enable);
