#include "Precompiled.h"

#include <iostream>

#include "AIStateMachine.h"
#include "WaypointAgentComponent.h"
#include <Game Engine/Scene/Component/Native Components/Physics/RigidbodyComponent.h>
#include "AIHordeReasoner.h"

#include "WalkState.h"
#include <AI/AIHordelingTypeComponent.h>
#include <PlayerLogic/PlayerIDComponent.h>


using namespace ursine::ecs;
namespace ursine
{
    namespace sm
    {
        WalkState::WalkState(std::string name)
            : AIState(name)
            , m_move(nullptr)
            , m_ghostCollider(nullptr)
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

            // we gon get ourselves a ghost collider
            m_ghostCollider = aiActor->GetComponentInChildren<ecs::Ghost>();
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

            // using this to average ouf the position
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

                // toss out this actor if they are too far
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

            // get our separation vector
            sepVec /= static_cast<float>( neighborhoodSize );

            // get this vector back into lengths [0 -> 1]
            sepVec /= m_nearRadius;

            // Get Cohersion vector
            avgPos /= static_cast<float>(neighborhoodSize);

            auto centerVec = avgPos - aiActorPos;

            auto centerLen = centerVec.Length();
            if (centerLen < m_nearRadius / 2)
            {
                centerLen = 0.0f;
            }
            else if (centerLen > m_nearRadius)
            {
                centerLen = m_nearRadius;
            }

            centerVec /= m_nearRadius;

            // get the composite vector using the scalars set by the component
            auto totalBoidVec = centerVec * m_coheasionScale + sepVec * m_separationScale;

            // apply to movement
            setTargetDirection(totalBoidVec + getTargetPlayerPosition(aiTrans->GetOwner()->GetWorld()));
        }

        void WalkState::Exit(AIStateMachine *stateMachine)
        {
            std::cout << "exited walk state" << std::endl;

        }

        void WalkState::updateSurroundingActorsList(void)
        {
            m_surrounding.clear();

            std::vector<physics::GhostOverlappingItem> pairs;
            // We have already got the ghost collider, just request collisions from it
            m_ghostCollider->GetOverlappingPairs( pairs );

            for (auto pair : pairs)
            {
                // if we are overlapping with something that isn't even dynamic, skip it
                if (pair.type != physics::BodyType::BT_RIGID_BODY)
                {
                    continue;
                }

                // we only care if the rigidbody belings to a hordeling
                if (!pair.rigidbody->GetOwner()->HasComponent<AIHordelingType>())
                {
                    continue;
                }

                m_surrounding.push_back(pair.rigidbody->GetOwner());

            }
        }

        void WalkState::setTargetDirectionWaypoints(const SVec3& target)
        {

        }

        void WalkState::setTargetDirectionMovement(const SVec3& target)
        {
            m_move->SetTargetDirection(target);
        }

        Vec3 WalkState::getTargetPlayerPosition(ecs::World *world)
        {
            // try to grab the player as the target
            auto players = world->GetEntitiesFromFilter(Filter().All< PlayerID >());

            Entity *target = nullptr;

            // if we can't find a player, just go to the closest waypoint
            if (players.size() < 1)
            {
                // if we can't find either, don't bother updating the targets
                auto selected = world->GetEntitiesFromFilter(Filter().All< Waypoint >());

                if (selected.size() < 1)
                {
                    return Vec3::Zero();
                }

                target = selected[0];
            }
            else
            {
                target = players[0];
            }

            return target->GetTransform()->GetWorldPosition();
        }
    }
}
