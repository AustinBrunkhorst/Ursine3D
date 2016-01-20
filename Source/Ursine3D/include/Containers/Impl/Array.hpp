#pragma once

#if defined(URSINE_ARRAY_NOTIFY_MODIFICATION)

#include "Variant.h"

namespace ursine
{
    struct ArrayModificationArgs : EventArgs
    {
        ArrayModifyAction action;
        size_t index;
        meta::Variant value;

        ArrayModificationArgs(ArrayModifyAction action, size_t index, const meta::Variant &value = nullptr)
            : action( action )
            , index( index )
            , value( value ) { }
    };
}

#define NOTIFY_MODIFICATION(index, action, ...)                        \
    ursine::ArrayModificationArgs __e( action, index, ##__VA_ARGS__ ); \
                                                                       \
    m_modifyEvents.Dispatch( action, this, &__e );                     \

#else

#define NOTIFY_MODIFICATION(index, action)

#endif

namespace ursine
{
    template<typename T>
    Array<T>::Array(void)
    {

    }

    template<typename T>
    Array<T>::Array(const Array &rhs)
        : m_impl( rhs.m_impl )
    {

    }

    template<typename T>
    Array<T>::Array(const Array &&rhs)
        : m_impl( std::move( rhs.m_impl ) )
    {

    }

    template<typename T>
    Array<T>::Array(const std::vector<T> &rhs)
        : m_impl( rhs )
    {

    }

    template<typename T>
    Array<T>::Array(const std::initializer_list<T> &rhs)
        : m_impl( rhs )
    {

    }

    template<typename T>
    Array<T>::Array(const std::initializer_list<T> &&rhs)
        : m_impl( std::move( rhs ) )
    {

    }

    template<typename T>
    typename Array<T>::Reference Array<T>::operator[](SizeType index)
    {
        return m_impl[ index ];
    }

    template<typename T>
    typename Array<T>::ConstReference Array<T>::operator[](SizeType index) const
    {
        return m_impl[ index ];
    }

    template<typename T>
    Array<T> &Array<T>::operator=(const Array &rhs)
    {
        m_impl = rhs.m_impl;

        return *this;
    }

    template<typename T>
    Array<T> &Array<T>::operator=(const Array &&rhs)
    {
        m_impl = std::move( rhs.m_impl );

        return *this;
    }

    template<typename T>
    typename Array<T>::Iterator Array<T>::begin(void)
    {
        return m_impl.begin( );
    }

    template<typename T>
    typename Array<T>::ConstIterator Array<T>::begin(void) const
    {
        return m_impl.begin( );
    }

    template<typename T>
    typename Array<T>::ConstIterator Array<T>::cbegin(void) const
    {
        return m_impl.cbegin( );
    }

    template<typename T>
    typename Array<T>::Iterator Array<T>::end(void)
    {
        return m_impl.end( );
    }

    template<typename T>
    typename Array<T>::ConstIterator Array<T>::end(void) const
    {
        return m_impl.end( );
    }

    template<typename T>
    typename Array<T>::ConstIterator Array<T>::cend(void) const
    {
        return m_impl.cend( );
    }

    template<typename T>
    void Array<T>::Push(const T &value)
    {
        m_impl.push_back( value );

        NOTIFY_MODIFICATION( m_impl.size( ) - 1, AMODIFY_INSERT, value );
    }

    template<typename T>
    void Array<T>::Push(const T &&value)
    {
        m_impl.emplace_back( std::move( value ) );

        NOTIFY_MODIFICATION( m_impl.size( ) - 1, AMODIFY_INSERT, value );
    }

    template<typename T>
    void Array<T>::Insert(SizeType index, const T &value)
    {
        m_impl.insert( begin( ) + index, value );

        NOTIFY_MODIFICATION( index, AMODIFY_INSERT, value );
    }

    template<typename T>
    void Array<T>::Insert(SizeType index, const T &&value)
    {
        m_impl.emplace( begin( ) + index, std::move( value ) );

        NOTIFY_MODIFICATION( index, AMODIFY_INSERT, value );
    }

    template<typename T>
    void Array<T>::Insert(ConstIterator position, const T &value)
    {
        m_impl.insert( position, value );

        NOTIFY_MODIFICATION( position - m_impl.cbegin( ), AMODIFY_INSERT, value );
    }

    template<typename T>
    void Array<T>::Insert(ConstIterator position, const T &&value)
    {
        m_impl.emplace( position, std::move( value ) );

        NOTIFY_MODIFICATION( position - m_impl.cbegin( ), AMODIFY_INSERT, value );
    }

    template<typename T>
    void Array<T>::Remove(SizeType index)
    {
        m_impl.erase( begin( ) + index );

        NOTIFY_MODIFICATION( index, AMODIFY_REMOVE );
    }

    template<typename T>
    void Array<T>::Remove(ConstIterator item)
    {
        m_impl.erase( item );

        NOTIFY_MODIFICATION( item - m_impl.cbegin( ), AMODIFY_REMOVE );
    }

    template<typename T>
    void Array<T>::Remove(ConstIterator begin, ConstIterator end)
    {
    #if defined(URSINE_ARRAY_NOTIFY_MODIFICATION)

        SizeType startIndex = begin - m_impl.cbegin( );
        SizeType count = end - begin;

    #endif

        m_impl.erase( begin, end );

    #if defined(URSINE_ARRAY_NOTIFY_MODIFICATION)

        for (SizeType i = 0; i < count; ++i)
        {
            NOTIFY_MODIFICATION( startIndex, AMODIFY_REMOVE );
        }

    #endif
    }

    template<typename T>
    void Array<T>::Remove(const T &value)
    {
        auto find = Find( value );

        if (find == end( ))
            return;

    #if defined(URSINE_ARRAY_NOTIFY_MODIFICATION)

        SizeType index = begin - m_impl.cbegin( );

    #endif

        m_impl.erase( find );

        NOTIFY_MODIFICATION( index, AMODIFY_REMOVE );
    }

    template<typename T>
    T Array<T>::Pop(void)
    {
        auto back = m_impl.back( );

        m_impl.pop_back( );

        NOTIFY_MODIFICATION( m_impl.size( ), AMODIFY_REMOVE );

        return back;
    }

    template<typename T>
    typename Array<T>::Reference Array<T>::GetValue(SizeType index)
    {
        return m_impl[ index ];
    }

    template<typename T>
    typename Array<T>::ConstReference Array<T>::GetValue(SizeType index) const
    {
        return m_impl[ index ];
    }

    template<typename T>
    void Array<T>::SetValue(SizeType index, const T &value)
    {
        m_impl[ index ] = value;

        NOTIFY_MODIFICATION( index, AMODIFY_SET, value );
    }

    template<typename T>
    void Array<T>::SetValue(SizeType index, const T &&value)
    {
        m_impl[ index ] = std::move( value );

        NOTIFY_MODIFICATION( index, AMODIFY_SET, value );
    }

    template<typename T>
    typename Array<T>::Iterator Array<T>::Find(const T &value)
    {
        return std::find( m_impl.begin( ), m_impl.end( ), value );
    }

    template<typename T>
    typename Array<T>::ConstIterator Array<T>::Find(const T &value) const
    {
        return std::find( m_impl.begin( ), m_impl.end( ), value );
    }

    template<typename T>
    typename Array<T>::SizeType Array<T>::Size(void) const
    {
        return m_impl.size( );
    }
}