#include "Precompiled.h"
//#include "Entity.h"
#include "AIReasonerComponent.h"
namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION(AIReasoner);


        AIReasoner::AIReasoner(void) : BaseComponent()
            , m_trans(nullptr)
        {
        }

        void AIReasoner::OnInitialize()
        {
            Component::OnInitialize();

            m_trans = GetOwner()->GetTransform();
        }
    }
}
