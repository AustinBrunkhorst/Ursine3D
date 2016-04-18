/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** GlobalGetterBase.h
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
        class GlobalGetterBase
        {
        public:
            virtual ~GlobalGetterBase(void) { }

            virtual Variant GetValue(void) = 0;
        };
    }
}