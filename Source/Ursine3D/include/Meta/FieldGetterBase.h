/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** FieldGetterBase.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    namespace meta
    {
        class FieldGetterBase
        {
        public:
            virtual ~FieldGetterBase(void) { }

            virtual Variant GetValue(const Variant &obj) = 0;
            virtual Variant GetValueReference(const Variant &obj) = 0;
        };
    }
}