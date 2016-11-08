/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Array.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <vector>

#include "ArrayConfig.h"

namespace ursine
{
    namespace meta
    {
        template<typename T>
        class ArrayWrapperContainer;
    }

    template<typename T>
    class Array
    {
    public:
        typedef typename std::vector<T>::size_type SizeType;

        typedef typename std::vector<T>::iterator Iterator;
        typedef typename std::vector<T>::const_iterator ConstIterator;

        typedef typename std::vector<T>::reference Reference;
        typedef typename std::vector<T>::reference ConstReference;

        Array(void);
        Array(const Array &rhs);
        Array(const Array &&rhs);
        Array(const std::vector<T> &rhs);
        Array(const std::initializer_list<T> &rhs);
        Array(const std::initializer_list<T> &&rhs);

        Reference operator[](SizeType index);
        ConstReference operator[](SizeType index) const;

        Array &operator=(const Array &rhs);
        Array &operator=(const Array &&rhs);
        
        Iterator begin(void);
        ConstIterator begin(void) const;
        ConstIterator cbegin(void) const;

        Iterator end(void);

        ConstIterator end(void) const;
        ConstIterator cend(void) const;

        void Push(const T &value);
        void Push(const T &&value);

        void Insert(SizeType index, const T &value);
        void Insert(SizeType index, const T &&value);
        void Insert(ConstIterator position, const T &value);
        void Insert(ConstIterator position, const T &&value);

        void Remove(SizeType index);
        void Remove(ConstIterator position);
        void Remove(ConstIterator begin, ConstIterator end);
        void Remove(const T &value);

        T Pop(void);

        void Clear(void);

        Reference GetValue(SizeType index);
        ConstReference GetValue(SizeType index) const;

        void SetValue(SizeType index, const T &value);
        void SetValue(SizeType index, const T &&value);

        Iterator Find(const T &value);
        ConstIterator Find(const T &value) const;

        bool Exists(const T &value) const;

        SizeType Size(void) const;

        void Resize(SizeType size);

    #if defined(URSINE_ARRAY_NOTIFY_MODIFICATION)

        ArrayEventDispatcher &GetModifyEvents(void);

    #endif

    private:

        std::vector<T> m_impl;

    #if defined(URSINE_ARRAY_NOTIFY_MODIFICATION)

        ArrayEventDispatcher m_modifyEvents;

    #endif
    };
}

#include "Impl/Array.hpp"