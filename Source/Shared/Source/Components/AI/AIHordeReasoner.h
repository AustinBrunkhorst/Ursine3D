/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AIHordeReasoner.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
**
** Notes: This is specifically for hardcoding the basic horde AI
** -------------------------------------------------------------------------*/
#pragma once

#include "Component.h"
#include "AIMovementControllerComponent.h"


namespace ursine
{
    namespace ecs
    {
        class AIHorde : public Component
        {
            NATIVE_COMPONENT;

        public:

            enum EnemyType
            {
                Hordeling,
                BigHordeling,
                Boomling,
                INVALID_ENEMY
            };

            AIHorde(void);

            void OnInitialize(void) override;

            void SetTarget(const Vec3 &pos);

            EnemyType GetEnemyType(void) const;
            void SetEnemyType(EnemyType newType);

            ////////////////////////////////////////////////////////////////////
            // Expose data to editor
            ////////////////////////////////////////////////////////////////////
            EditorField(
                EnemyType TypeOfEnemy,
                GetEnemyType,
                SetEnemyType
                );

        private:
            Handle<AIMovementController> m_move;

            EnemyType m_type;

        }Meta(Enable, DisplayName("AIHordeReasoner"));

    }
}
