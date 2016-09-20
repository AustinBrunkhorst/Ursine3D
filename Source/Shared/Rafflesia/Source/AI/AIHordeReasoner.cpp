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
            , m_type(EnemyType::HORDELING)
        {
        }

        void AIHorde::OnInitialize()
        {
            Component::OnInitialize();

            m_move = GetOwner()->GetComponent<AIMovementController>();
        }

        // NOTE: this actually can't be const due to weird handle stuff
        void AIHorde::SetTarget(const SVec3& pos)
        {
            m_move->SetTargetDirection(pos - GetOwner()->GetTransform()->GetWorldPosition());
        }

        EnemyType AIHorde::GetEnemyType(void) const
        {
            return m_type;
        }

        void AIHorde::SetEnemyType(EnemyType newType)
        {
            m_type = newType;
        }
    }
}
