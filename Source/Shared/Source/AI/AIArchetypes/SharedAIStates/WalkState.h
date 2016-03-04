/* ---------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** WalkState.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** -------------------------------------------------------------------------*/
#pragma once

#include "AIState.h"
#include "AI/AIMovementControllerComponent.h"
#include "Entity.h"
#include "Vec3.h"

#include <function.h>
#include <Game Engine/Scene/Component/Native Components/Physics/GhostComponent.h>

class AIStateMachine;

namespace ursine
{
    namespace sm
    {
        class WalkState : public AIState
        {
        public:

            WalkState(std::string name);

            virtual void Enter(AIStateMachine *stateMachine) override;
            virtual void Update(AIStateMachine *stateMachine) override;
            virtual void Exit(AIStateMachine *stateMachine) override;

            void SetNearRadius(float radius);

            void SetCohesionScale(float scale);

            void SetSeparationScale(float scale);

            void SetBoidbehaviorScale(float scale);

            void SetAttackRange(float range);
        private:

            void updateSurroundingActorsList(AIStateMachine *stateMachine);

            void setTargetDirectionWaypoints(const SVec3 &target);
            void setTargetDirectionMovement(const SVec3 &target);

            Vec3 getTargetPlayerPosition(ecs::World *world);

            typedef std::function<void(const SVec3&)> setTargetDirectionFunc;
            setTargetDirectionFunc setTargetDirection;

            std::vector<ecs::Entity *> m_surrounding;

            ecs::AIMovementController *m_move;

            ecs::Ghost *m_ghostCollider;

            float m_nearRadius;

            float m_attackRange;

            float m_cohesionScale;

            float m_separationScale;

            float m_boidBehaviorScale;
        };

    }
}
