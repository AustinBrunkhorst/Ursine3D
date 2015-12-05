/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ScoreSystem.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <EntitySystem.h>


        class ScoreSystem : public ursine::ecs::EntitySystem
        {
            ENTITY_SYSTEM;

        public:
            ScoreSystem(ursine::ecs::World *world);

        private:

            int m_player1Kills;
            int m_player2Kills;
            
            int m_player1Wins;
            int m_player2Wins;
            
            int m_currRound;
            int m_playerCount;
            int m_maxPlayerCount;

        } Meta(Enable);
