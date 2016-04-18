/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** MethodInvokerBase.h
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
        class MethodInvokerBase
        {
        public:
            virtual ~MethodInvokerBase(void) { }

            virtual Variant Invoke(Variant &obj, const ArgumentList &arguments) = 0;
        };
    }
}