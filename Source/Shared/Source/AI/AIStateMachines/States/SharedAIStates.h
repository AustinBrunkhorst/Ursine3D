// this file is used to store any AIstates that can be shared across actors

#include "AIState.h"
#include "Entity.h"
#include "Vec3.h"

#include <function.h>

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
            
            //typedef std::_Binder<void, void (ursine::sm::WalkState::*)(const ursine::Vec3 &target), ursine::sm::WalkState *> targetDirectionfn;
        private:

            std::vector<ecs::Entity *> &getSurroundingActors(const ecs::Entity *actor);

            void setTargetDirectionWaypoints(const SVec3 &target);
            void setTargetDirectionMovement(const SVec3 &target);

            typedef std::function<void(const SVec3&)> setTargetDirectionFunc;
            setTargetDirectionFunc setTargetDirection;

            float m_nearRadius;
        };
    }
}