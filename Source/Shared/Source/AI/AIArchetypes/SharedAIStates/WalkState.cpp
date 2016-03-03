#include "Precompiled.h"

#include <iostream>

#include "AIStateMachine.h"
#include "WaypointAgentComponent.h"


#include <Game Engine/Scene/Component/Native Components/Physics/RigidbodyComponent.h>
#include "WalkState.h"

using namespace ursine::ecs;
namespace ursine
{
    namespace sm
    {
        WalkState::WalkState(std::string name)
            : AIState(name)
            , m_move(nullptr)
            , m_nearRadius(1.0f)
        {

        }

        void WalkState::Enter(AIStateMachine *stateMachine)
        {
            using namespace std::placeholders;

            std::cout << "entered walk state" << std::endl;

            // first get the entity
            Entity *aiActor = stateMachine->GetEntity();

            //then check if we have a waypoint controller or not and set target function accordingly
            if (aiActor->HasComponent<WaypointAgent>())
            {
                setTargetDirection = std::bind(&WalkState::setTargetDirectionWaypoints, this, _1);

            }
            else
            {
                setTargetDirection = std::bind(&WalkState::setTargetDirectionMovement, this, _1);

                m_move = aiActor->GetComponent<AIMovementController>();
            }
        }

        void WalkState::Update(AIStateMachine *stateMachine)
        {
            std::cout << "updated walk state" << std::endl;

            auto aiTrans = stateMachine->GetEntity()->GetTransform();
            Vec3 aiActorPos = aiTrans->GetWorldPosition();

            //auto *aiActorRigid = stateMachine->GetEntity()->GetComponent<Rigidbody>();

            // Separation:
            Vec3 sepVec(0, 0, 0);

            // we are going to use this for getting average position of local boids
            Vec3 avgPos = aiActorPos;

            //Vec3 avgFacing = aiTrans->GetForward();

            // using this to average out the position
            int neighborhoodSize = 1;

            for (auto actor : m_surrounding)
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

            // get the averages for facing (and Cohersion if we use it)
            avgPos /= static_cast<float>( neighborhoodSize );
            //avgFacing /= neighborhoodSize;

            // now that we have the averages, we apply them
            // NOTE: at some point we will add option in editor for scales

            // apply to movement

        }

        void WalkState::Exit(AIStateMachine *stateMachine)
        {
            std::cout << "exited walk state" << std::endl;

        }

        void WalkState::updateSurroundingActorsList(const ecs::Entity* actor)
        {
        }

        void WalkState::setTargetDirectionWaypoints(const SVec3& target)
        {

        }

        void WalkState::setTargetDirectionMovement(const SVec3& target)
        {
            m_move->SetTargetDirection(target);
        }
    }
}
