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

// here is a fancy macro for quickly passing component members to this state
#define INIT_WALKSTATE(name)                                              \
auto walkState = m_stateMachine.AddState<sm::WalkState>(name);            \
do{ walkState->SetCohesionScale(m_cohesionScale);                         \
walkState->SetSeparationScale(m_separationScale);                         \
walkState->SetBoidbehaviorScale(m_boidScale);                             \
walkState->SetAttackRange(m_attackRange);                                 \
m_stateMachine.SetInitialState(walkState);                                \
}while (0)


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

            ecs::AIMovementController *GetMovementController(void) const;
        private:

            void updateSurroundingActorsList(AIStateMachine *stateMachine);

            void setTargetDirectionWaypoints(const SVec3 &target);
            void setTargetDirectionMovement(const SVec3 &target);

            typedef std::function<void(const SVec3&)> setTargetDirectionFunc;
            setTargetDirectionFunc setTargetDirection;

            std::vector<ecs::EntityHandle> m_surrounding;

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
