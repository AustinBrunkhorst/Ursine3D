/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** DestructorInvokerBase.h
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
        class DestructorInvokerBase
        {
        public:
            virtual ~DestructorInvokerBase(void) { }

            virtual void Invoke(const Variant &obj) = 0;
        };
    }
}