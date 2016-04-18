/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ConstructorInvoker.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ConstructorInvokerBase.h"

namespace ursine
{
    namespace meta
    {
        template<typename ClassType, bool IsDynamic, bool IsObjectWrapped, typename ...ArgTypes>
        class ConstructorInvoker : public ConstructorInvokerBase
        {
        public:
            static const size_t ArgCount = sizeof...( ArgTypes );

            static_assert( ArgCount <= MaxArgumentCount,
                "Constructor has too many arguments. It's time to generate more overloads." 
            );

            Variant Invoke(const ArgumentList &arguments) override;
        };
    }
}

#include "Impl/ConstructorInvoker.hpp"