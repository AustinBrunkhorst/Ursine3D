// this file is used to store any AIstates that can be shared across actors

#include "AIState.h"
#include "Entity.h"
#include "Vec3.h"

#include <function.h>
#include <AI/AIMovementControllerComponent.h>

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

        private:

            void updateSurroundingActorsList(const ecs::Entity *actor);

            void setTargetDirectionWaypoints(const SVec3 &target);
            void setTargetDirectionMovement(const SVec3 &target);

            typedef std::function<void(const SVec3&)> setTargetDirectionFunc;
            setTargetDirectionFunc setTargetDirection;

            std::vector<ecs::Entity *> m_surrounding;

            ecs::AIMovementController *m_move;

            float m_nearRadius;
        };

    }
}
