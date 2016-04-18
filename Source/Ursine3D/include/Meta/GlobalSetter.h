/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** GlobalSetter.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "GlobalSetterBase.h"

namespace ursine
{
    namespace meta
    {
        template<typename GlobalType, bool IsMethod>
        class GlobalSetter : public GlobalSetterBase
        {
        public:
            void SetValue(const Argument &value) override;
        };
    }
}

#include "Impl/GlobalSetter.hpp"