#pragma once

#include <EntitySystem.h>


        class ScoreSystem : public ursine::ecs::EntitySystem
        {
            ENTITY_SYSTEM;

        public:
            ScoreSystem(ursine::ecs::World *world);

        private:
            void OnInitialize(void) override;
            void OnRemove(void) override;

            int m_player1Kills;
            int m_player2Kills;
            
            int m_player1Wins;
            int m_player2Wins;
            
            int m_currRound;
            int m_playerCount;
            int m_maxPlayerCount;

        } Meta(Enable);
