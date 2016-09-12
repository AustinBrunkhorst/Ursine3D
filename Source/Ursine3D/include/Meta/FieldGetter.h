/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** FieldGetter.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "FieldGetterBase.h"

namespace ursine
{
    namespace meta
    {
        template<typename ClassType, typename FieldType, bool IsMethod>
        class FieldGetter : public FieldGetterBase
        {
        public:
            Variant GetValue(const Variant &obj) override;
            Variant GetValueReference(const Variant &obj) override;
        };
    }
}

#include "Impl/FieldGetter.hpp"