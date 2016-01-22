/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ArrayWrapper.h
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
        class ArrayWrapper
        {
        public:
            ArrayWrapper(void);

            template<typename T>
            ArrayWrapper(Array<T> &rhs);

            Variant GetValue(size_t index) const;
            void SetValue(size_t index, const Argument &value);

            void Insert(size_t index, const Argument &value);
            void Remove(size_t index);

            size_t Size(void) const;

            bool IsValid(void) const;

        private:
            ArrayWrapperBase *m_base;
        };
    }
}

#include "Impl/ArrayWrapper.hpp"