/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** FieldSetter.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "FieldSetterBase.h"

namespace ursine
{
    namespace meta
    {
        template<typename ClassType, typename FieldType, bool IsMethod>
        class FieldSetter : public FieldSetterBase
        {
        public:
            void SetValue(Variant &obj, const Variant &value) override;
        };
    }
}

#include "Impl/FieldSetter.hpp"