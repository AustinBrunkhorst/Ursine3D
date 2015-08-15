/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SystemType.hpp
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
        template<class Type>
        SystemTypeID SystemType<Type>::ID = 0;

        template<class Type>
        SystemTypeID SystemType<Type>::Mask = 0;

        template<class Type>
        bool SystemType<Type>::Registered = false;

        template<class Type>
        SystemFactory SystemType<Type>::Factory = nullptr;

        template<class Type>
        SystemType<Type>::SystemType(const char *name, SystemFactory factory, bool auto_add)
        {
            SystemRegistrar::Register<Type>(name, factory, auto_add);
        }
    }
}