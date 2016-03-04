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

        void AIStateMachine::Initialize(ecs::Entity* actor)
        {
            m_actorEntity = actor;
        }

        ecs::Entity* AIStateMachine::GetEntity() const
        {
            return m_actorEntity;
        }
    }
}