/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ConstructorInvokerBase.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "InvokableConfig.h"

namespace ursine
{
    namespace meta
    {
        class ConstructorInvokerBase
        {
        public:
            virtual ~ConstructorInvokerBase(void) { }

            virtual Variant Invoke(const ArgumentList &arguments) = 0;
        };
    }
}