#include "Precompiled.h"

#include <iostream>

#include "AIStateMachine.h"
#include "WaypointAgentComponent.h"
#include <Game Engine/Scene/Component/Native Components/Physics/RigidbodyComponent.h>
#include "AIHordeReasoner.h"

#include "WalkState.h"
#include <AI/AIHordelingTypeComponent.h>
#include <PlayerLogic/PlayerIDComponent.h>
#include <Game Engine/Scene/Component/Native Components/Physics/SphereColliderComponent.h>


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
            , m_attackRange(3.0f)
            , m_cohesionScale(0.5f)
            , m_separationScale(0.5f)
        {

        }

        void WalkState::Enter(AIStateMachine *stateMachine)
        {
            using namespace std::placeholders;

            // first get the entity
            EntityHandle aiActor = stateMachine->GetEntity();

            stateMachine->SetBool("HitPlayer", false);
            
            //then check if we have a waypoint controller or not and set target function accordingly
            if (aiActor->HasComponent<WaypointAgent>())
            {
                setTargetDirection = std::bind(&WalkState::setTargetDirectionWaypoints, this, _1);

            }
            else
            {
                setTargetDirection = std::bind(&WalkState::setTargetDirectionMovement, this, _1);

            }

            m_move = aiActor->GetComponent<AIMovementController>();

            // we gon get ourselves a ghost collider
            m_ghostCollider = aiActor->GetComponentInChildren<ecs::Ghost>();

            UAssert(m_ghostCollider, "Child of Entity With walkState must have a ghost collider");

            SetNearRadius(m_ghostCollider->GetOwner()->GetComponent<SphereCollider>()->GetRadius());

            m_move->SetEnable(true);
        }

        void WalkState::Update(AIStateMachine *stateMachine)
        {
            updateSurroundingActorsList(stateMachine);

            auto aiTrans = stateMachine->GetEntity()->GetTransform();
            Vec3 aiActorPos = aiTrans->GetWorldPosition();

            // if we are in range to attack the player, we need to switch to the attack state
            auto playerPos = GetTargetPlayerPosition(aiTrans->GetOwner()->GetWorld());

            auto playerDirection = playerPos - aiActorPos;

            if (playerDirection.Length() <= m_attackRange)
            {
                stateMachine->SetBool("HitPlayer", true);
                return;
            }

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
            auto totalBoidVec = centerVec * m_cohesionScale + -sepVec * m_separationScale;

            //auto playerDirection = playerPos - aiActorPos;

            playerDirection.Normalize();

            // apply to movement
            setTargetDirection(  playerDirection );

            aiTrans->LookAt( SVec3( playerPos.X(), aiTrans->GetWorldPosition().Y(), playerPos.Z() ) );
        }

        void WalkState::Exit(AIStateMachine *stateMachine)
        {
            m_move->SetEnable(false);
        }

        void WalkState::SetNearRadius(float radius)
        {
            m_nearRadius = radius;
        }

        void WalkState::SetCohesionScale(float scale)
        {
            m_cohesionScale = scale;
        }

        void WalkState::SetSeparationScale(float scale)
        {
            m_separationScale = scale;
        }

        void WalkState::SetBoidbehaviorScale(float scale)
        {
            m_boidBehaviorScale = scale;
        }

        void WalkState::SetAttackRange(float range)
        {
            m_attackRange = range;
        }

        void WalkState::updateSurroundingActorsList(AIStateMachine *stateMachine)
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

        ecs::AIMovementController* WalkState::GetMovementController(void) const
        {
            return m_move;
        }
    }
}
