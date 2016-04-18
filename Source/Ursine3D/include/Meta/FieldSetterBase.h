/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** FieldSetterBase.h
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
        class FieldSetterBase
        {
        public:
            virtual ~FieldSetterBase(void) { }

            virtual void SetValue(Variant &obj, const Variant &value) = 0;
        };
    }
}