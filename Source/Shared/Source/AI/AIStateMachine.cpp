#include "Precompiled.h"

#include "AIStateMachine.h"
namespace ursine
{
    namespace sm
    {
        AIStateMachine::AIStateMachine(ecs::Entity *aiActor): StateMachine(nullptr)
                                                            , m_actorEntity(aiActor)
        {
            
        }

        ecs::Entity* AIStateMachine::GetEntity() const
        {
            return m_actorEntity;
        }
    }
}