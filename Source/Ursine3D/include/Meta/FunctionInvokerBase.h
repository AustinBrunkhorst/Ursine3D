/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** FunctionInvokerBase.h
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
        class FunctionInvokerBase
        {
        public:
            virtual ~FunctionInvokerBase(void) { }

            virtual Variant Invoke(const ArgumentList &arguments) = 0;
        };
    }
}