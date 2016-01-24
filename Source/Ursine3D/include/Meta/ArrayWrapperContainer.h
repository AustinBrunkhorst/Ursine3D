/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ArrayWrapperContainer.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ArrayWrapperBase.h"
#include "Array.h"

namespace ursine
{
    namespace meta
    {
        template<typename T>
        class ArrayWrapperContainer : public ArrayWrapperBase
        {
        public:
            ArrayWrapperContainer(Array<T> &a);

            Variant GetValue(size_t index) override;
            void SetValue(size_t index, const Argument &value) override;

            void Insert(size_t index, const Argument &value) override;
            void Remove(size_t index) override;

            size_t Size(void) const override;

        #if defined(URSINE_WITH_EDITOR)

            ArrayEventDispatcher &GetModifyEvents(void) const override;

        #endif

        private:
            Array<T> &m_array;
        };
    }
}

#include "Impl/ArrayWrapperContainer.hpp"