#include "Precompiled.h"
//#include "Entity.h"
#include "AIHordelingTypeComponent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION(AIHordelingType);


        AIHordelingType::AIHordelingType(void)
            : BaseComponent()
            , m_type(HordelingTypeID::INVALID_TYPE)
        {         
        }

        AIHordelingType::AIHordelingType(HordelingTypeID type)
            : BaseComponent()
            , m_type(type)
        {
        }

        AIHordelingType::HordelingTypeID AIHordelingType::GetHordelingID() const
        {
            return m_type;
        }

        void AIHordelingType::SetHordelingID(HordelingTypeID type)
        {
            m_type = type;
        }
    }
}
