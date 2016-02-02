/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ArrayWrapperBase.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Type.h"
#include "UrsineTypes.h"

#include "ArrayConfig.h"

namespace ursine
{
    namespace meta
    {
        class Variant;
        class Argument;

        class ArrayWrapperBase
        {
        public:
            virtual ~ArrayWrapperBase(void) { }

            virtual Variant GetValue(size_t index) = 0;
            virtual void SetValue(size_t index, const Argument &value) = 0;

            virtual void Insert(size_t index, const Argument &value) = 0;
            virtual void Remove(size_t index) = 0;

            virtual size_t Size(void) const = 0;
    
        #if defined(URSINE_WITH_EDITOR)

            virtual ArrayEventDispatcher &GetModifyEvents(void) const = 0;

        #endif
        };
    }
}