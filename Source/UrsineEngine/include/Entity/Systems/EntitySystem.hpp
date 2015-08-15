/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntitySystem.hpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/


#include "SystemRegistrar.h"

namespace Ursine
{
    namespace ECS
    {
        inline SystemTypeID EntitySystem::GetTypeID(void)
        {
            return _type_id;
        }

        inline SystemTypeMask EntitySystem::GetTypeMask(void)
        {
            return _type_mask;
        }

        template<class SystemType>
        inline bool EntitySystem::Is(void)
        {
            return _type_id == GetEntitySystemID(SystemType);
        }
    }
}