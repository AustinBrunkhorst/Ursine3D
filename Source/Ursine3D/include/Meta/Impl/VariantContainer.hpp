#include "Type.h"

#pragma warning(push)

// unused template parameters
#pragma warning(disable : 4544)

// implicit conversion performance warnings
#pragma warning(disable : 4800)

// possible loss of data
#pragma warning(disable : 4244)

#define DEFAULT_TYPE_HANDLER_IMPL(typeName)                                                      \
    template<typename T>                                                                         \
    template<typename U = T>                                                                     \
    typeName VariantContainer<T>::get##typeName(                                                 \
        typename std::enable_if<                                                                 \
            !std::is_convertible<typename meta_traits::TypeOrEnumType<U>::type, typeName>::value \
        >::type* = nullptr                                                                       \
        ) const                                                                                  \
    {                                                                                            \
        return typeName( );                                                                      \
    }                                                                                            \
    template<typename T>                                                                         \
    template<typename U = T>                                                                     \
    typeName VariantContainer<T>::get##typeName(                                                 \
        typename std::enable_if<                                                                 \
            std::is_convertible<typename meta_traits::TypeOrEnumType<U>::type, typeName>::value  \
        >::type* = nullptr                                                                       \
    ) const                                                                                      \
    {                                                                                            \
        return static_cast<typeName>( m_value );                                                 \
    }                                                                                            \

namespace ursine
{
    namespace meta
    {
        template<typename T>
        VariantContainer<T>::VariantContainer(const NonRefType &value)
            : m_value( const_cast<NonRefType&>( value ) )
        {
        
        }

        ///////////////////////////////////////////////////////////////////////

        template<typename T>
        VariantContainer<T>::VariantContainer(const NonRefType &&value)
            : m_value( std::move( const_cast<NonRefType&&>( value ) ) )
        {
        
        }

        ///////////////////////////////////////////////////////////////////////

        template<typename T>
        Type VariantContainer<T>::GetType(void) const
        {
            return typeof( T );
        }

        ///////////////////////////////////////////////////////////////////////

        template<typename T>
        int VariantContainer<T>::ToInt(void) const
        {
            return getint( );
        }

        ///////////////////////////////////////////////////////////////////////

        template<typename T>
        bool VariantContainer<T>::ToBool(void) const
        {
            return getbool( );
        }

        ///////////////////////////////////////////////////////////////////////

        template<typename T>
        float VariantContainer<T>::ToFloat(void) const
        {
            return getfloat( );
        }

        ///////////////////////////////////////////////////////////////////////

        template<typename T>
        double VariantContainer<T>::ToDouble(void) const
        {
            return getdouble( );
        }

        ///////////////////////////////////////////////////////////////////////

        template<typename T>
        std::string VariantContainer<T>::ToString(void) const
        {
            return getString( );
        }

        ///////////////////////////////////////////////////////////////////////

        template<typename T>
        void *VariantContainer<T>::GetPtr(void) const
        {
            return const_cast<void*>(
                reinterpret_cast<const void*>( 
                    std::addressof( m_value )
                )
            );
        }

        ///////////////////////////////////////////////////////////////////////

        template<typename T>
        VariantBase *VariantContainer<T>::Clone(void) const
        {
            // use the non reference type to ensure a copy is made
            return new VariantContainer<NonRefType>( m_value );
        }

        template<typename T>
        void VariantContainer<T>::OnSerialize(Json::object &output) const
        {
            onSerialize( output );
        }

        template<typename T>
        void VariantContainer<T>::OnDeserialize(const Json &input)
        { 
            onDeserialize( input );
        }

        ///////////////////////////////////////////////////////////////////////

        DEFAULT_TYPE_HANDLER_IMPL( int );
        DEFAULT_TYPE_HANDLER_IMPL( bool );
        DEFAULT_TYPE_HANDLER_IMPL( float );
        DEFAULT_TYPE_HANDLER_IMPL( double );

        template<typename T>
        template<typename U = T>
        std::string VariantContainer<T>::getString(
            typename std::enable_if<
                !std::is_arithmetic<U>::value
            >::type* = nullptr
        ) const
        {
            return std::string( );
        }

        template<typename T>
        template<typename U = T>
        std::string VariantContainer<T>::getString(
            typename std::enable_if<
                std::is_arithmetic<U>::value
            >::type* = nullptr
        ) const
        {
            return std::to_string( m_value );
        }

        template<typename T>
        template<typename U>
        void VariantContainer<T>::onSerialize(
            Json::object &output, 
            typename std::enable_if<
                !std::is_pointer<U>::value && std::is_base_of<Object, U>::value
            >::type*
        ) const
        {
            m_value.OnSerialize( output );
        }
        
        template<typename T>
        template<typename U>
        void VariantContainer<T>::onSerialize(
            Json::object &output,
            typename std::enable_if<
            std::is_pointer<U>::value || !std::is_base_of<Object, U>::value
            >::type* = nullptr
        ) const
        {
            // do nothing
        }

        template<typename T>
        template<typename U>
        void VariantContainer<T>::onDeserialize(
            const Json &input,
            typename std::enable_if<
                !std::is_pointer<U>::value && std::is_base_of<Object, U>::value
            >::type*
        ) 
        {
            m_value.OnDeserialize( input );
        }
        
        template<typename T>
        template<typename U>
        void VariantContainer<T>::onDeserialize(
            const Json &input,
            typename std::enable_if<
                std::is_pointer<U>::value || !std::is_base_of<Object, U>::value
            >::type* = nullptr
        )
        {
            // do nothing
        }
    }
}

#undef DEFAULT_TYPE_HANDLER_IMPL

#pragma warning(pop)