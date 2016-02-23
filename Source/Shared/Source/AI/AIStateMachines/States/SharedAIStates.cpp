#include "Precompiled.h"

#include "AIStateMachine.h"
#include "WaypointAgentComponent.h"

#include "SharedAIStates.h"

#include <Game Engine/Scene/Component/Native Components/Physics/RigidbodyComponent.h>

using namespace ursine::ecs;
namespace ursine
{
    namespace sm
    {
        WalkState::WalkState(std::string name):AIState(name)
        {
            
        }

        void WalkState::Enter(AIStateMachine *stateMachine)
        {
            // first get the entity
            Entity *aiActor = stateMachine->GetEntity();

            //then check if we have a waypoint controller or not and set target function accordingly
            if (aiActor->HasComponent<WaypointAgent>())
            {
                setTargetDirection = reinterpret_cast<targetDirectionfn>(setTargetDirectionWaypoints);
            }
            else
            {
                setTargetDirection = reinterpret_cast<targetDirectionfn>(setTargetDirectionMovement);
            }
        }

        void WalkState::Update(AIStateMachine *stateMachine)
        {
            auto aiTrans = stateMachine->GetEntity()->GetTransform();
            Vec3 aiActorPos = aiTrans->GetWorldPosition();

            //auto *aiActorRigid = stateMachine->GetEntity()->GetComponent<Rigidbody>();

            auto actorList = getSurroundingActors(stateMachine->GetEntity());

            // Separation:
            Vec3 sepVec(0, 0, 0);

            // we are going to use this for getting average position of local boids
            Vec3 avgPos = aiActorPos;

            //Vec3 avgFacing = aiTrans->GetForward();

            // using this to average out the position
            int neighborhoodSize = 1;

            for (auto actor : actorList)
            {
                // ingnore this actor if they are us
                if (actor == stateMachine->GetEntity())
                {
                    continue;
                }

                Vec3 pos = actor->GetTransform()->GetWorldPosition();
                Vec3 distVec = pos - aiActorPos;

                float len = distVec.Length();

                // toss out this actor if they are too close
                if (len > m_nearRadius)
                {
                    continue;
                }

                // Separation:
                distVec.Normalize();
                distVec *= m_nearRadius - len;
                sepVec += distVec;

                // Cohesion:
                ++neighborhoodSize;
                avgPos += pos;

                // Direction:
                //avgFacing += actor->GetTransform()->GetForward();
            }

            // get the averages for facing and Coherasion
            avgPos /= neighborhoodSize;
            //avgFacing /= neighborhoodSize;

            // now that we have the averages, we apply them
            // NOTE: at some point we will add option in editor for scales

            // apply to movem

        }

        void WalkState::Exit(AIStateMachine *stateMachine)
        {

        }

    }
}