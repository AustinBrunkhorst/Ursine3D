#include "Precompiled.h"

#include "FodderAIStateMachine.h"

#include <CollisionEventArgs.h>
#include <EntityEvent.h>
#include <PlayerLogic/PlayerIdComponent.h>

namespace ursine
{
    namespace sm
    {
        FodderAIStateMachine::FodderAIStateMachine()
            :AIStateMachine()
        {
            AddBool("HitPlayer", false);
            AddFloat("PauseTimer", 0.0f);
        }

        void FodderAIStateMachine::Initialize(ecs::EntityHandle actor)
        {
            m_actorEntity = actor;

            m_actorEntity->Listener(this)
                .On(ursine::ecs::ENTITY_COLLISION_PERSISTED, &FodderAIStateMachine::onCollide);

            AIStateMachine::Initialize(actor);
        }

        FodderAIStateMachine::~FodderAIStateMachine()
        {
            if (m_actorEntity)
            {
                m_actorEntity->Listener(this)
                    .Off(ursine::ecs::ENTITY_COLLISION_PERSISTED, &FodderAIStateMachine::onCollide);
            }
        }

        void FodderAIStateMachine::onCollide(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED))
        {
            EVENT_ATTRS(ursine::ecs::Entity, ursine::physics::CollisionEventArgs);

            auto otherRoot = args->otherEntity->GetRoot();

            if (!otherRoot->HasComponent<PlayerID>())
            {
                return;
            }

            SetBool("HitPlayer", true);
        }
    }
}
