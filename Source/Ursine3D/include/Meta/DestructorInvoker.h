/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** DestructorInvoker.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "DestructorInvokerBase.h"

namespace ursine
{
    namespace meta
    {
        template<typename ClassType>
        class DestructorInvoker : public DestructorInvokerBase
        {
        public:
            void Invoke(const Variant &obj) override;
        };
    }
}

#include "Impl/DestructorInvoker.hpp"