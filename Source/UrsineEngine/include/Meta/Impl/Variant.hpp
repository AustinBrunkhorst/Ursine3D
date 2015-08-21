#include "VariantContainer.h"

namespace Ursine
{
    template<typename T>
    Variant::Variant(T &data)
        : m_base( new VariantContainer< CleanedType<T> >( data ) )
    {
        
    }

    ////////////////////////////////////////////////////////////////////////////

    template<typename T>
    Variant::Variant(T &&data)
        : m_base( new VariantContainer< CleanedType<T> >( static_cast<T&&>( data ) ) )
    {
        static_assert( !std::is_same< Variant&, T >::value, 
            "Variant cannot be used in r-value Variant constructor" );

        static_assert( !std::is_const< T >::value,
            "Type cannot be const" );
    }

    ////////////////////////////////////////////////////////////////////////////

    template<typename T>
    Variant &Variant::operator=(T &&rhs)
    {
        Variant( static_cast<T&&>( rhs ) ).Swap( *this );

        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////

    template<typename T>
    T &Variant::GetValue(void) const
    {
        typedef typename std::remove_cv<T>::type ContainerType;

        return static_cast< VariantContainer<ContainerType> *>( m_base )->m_value;
    }
}
