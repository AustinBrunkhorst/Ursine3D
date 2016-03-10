#include "Precompiled.h"

#include "AIStateMachine.h"
namespace ursine
{
    namespace sm
    {
        AIStateMachine::AIStateMachine(void): StateMachine( )
                                            , m_actorEntity(nullptr)
        {
        }

        void AIStateMachine::Initialize(ecs::EntityHandle actor)
        {
            m_actorEntity = actor;
        }

        ecs::EntityHandle AIStateMachine::GetEntity() const
        {
            return m_actorEntity;
        }
    }
}