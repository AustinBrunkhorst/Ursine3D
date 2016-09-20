/* ---------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BomberExplodeState.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once
#include <ArchetypeData.h>

#include "AIState.h"
#include "Entity.h"
#include "Vec3.h"
#include "WalkState.h"



class AIStateMachine;

class EntityAnimator;


namespace ursine
{
    namespace sm
    {
        class BomberExplodeState : public AIState
        {
        public:
            BomberExplodeState(const std::string &name, WalkState *walkState,float damage);

            bool CanExit(void) override { return m_finished; }

            void Enter(AIStateMachine *machine) override;
            void Update(AIStateMachine *machine) override;
            void Exit(AIStateMachine *machine) override;

            void SetExplosionArchetype(ursine::resources::ResourceReference &dmgExplosionEntity);

            void SetExplodeRange(float range);

        private:
            ursine::resources::ResourceReference m_objToSpawn;

            WalkState *m_walk;

            bool m_finished;

            float m_explodeRange;
        };

    }
}