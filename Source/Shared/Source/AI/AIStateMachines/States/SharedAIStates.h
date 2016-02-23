// this file is used to store any AIstates that can be shared across actors

#include "AIState.h"
#include "Entity.h"
#include "Vec3.h"

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

            typedef void(WalkState::*targetDirectionfn)();
        private:
            std::vector<ecs::Entity *> &getSurroundingActors(const ecs::Entity *actor);

            void setTargetDirectionWaypoints(const Vec3 &target);
            void setTargetDirectionMovement(const Vec3 &target);

            targetDirectionfn setTargetDirection;

            float m_nearRadius;
        };
    }
}