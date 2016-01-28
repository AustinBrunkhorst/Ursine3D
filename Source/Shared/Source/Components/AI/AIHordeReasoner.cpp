#include "Precompiled.h"
//#include "Entity.h"
#include "AIHordeReasoner.h"
namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION(AIHorde);


        AIHorde::AIHorde(void) : BaseComponent()
            , m_move(nullptr)
        {
        }

        void AIHorde::OnInitialize()
        {
            Component::OnInitialize();

            m_move = GetOwner()->GetComponent<AIMovementController>();
        }

        void AIHorde::SetTarget(const Vec3& pos) const
        {
            m_move->SetTargetDirection(pos - GetOwner()->GetTransform()->GetWorldPosition());
        }
    }
}
