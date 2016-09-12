/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** GlobalSetterBase.h
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
        class GlobalSetterBase
        {
        public:
            virtual ~GlobalSetterBase(void) { }

            virtual void SetValue(const Argument &value) = 0;
        };
    }
}