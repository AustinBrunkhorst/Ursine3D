#include "Type.h"

namespace ursine
{
    namespace meta
    {
        template<typename T>
        VariantContainer<T>::~VariantContainer(void)
        {
            delete m_value;
        }

        template<typename T>
        VariantContainer<T>::VariantContainer(const T &value)
            : m_value( new T( value ) )
        {
        
        }

        ////////////////////////////////////////////////////////////////////////////

        template<typename T>
        VariantContainer<T>::VariantContainer(const T &&value)
            : m_value( new T( std::move( value ) ) )
        {
        
        }

        ////////////////////////////////////////////////////////////////////////////

        template<typename T>
        Type VariantContainer<T>::GetType(void) const
        {
            return typeof( T );
        }

        ////////////////////////////////////////////////////////////////////////////

        template<typename T>
        int VariantContainer<T>::ToInt(void) const
        {
            return int( );
        }

        ////////////////////////////////////////////////////////////////////////////

        template<typename T>
        bool VariantContainer<T>::ToBool(void) const
        {
            return bool( );
        }

        ////////////////////////////////////////////////////////////////////////////

        template<typename T>
        float VariantContainer<T>::ToFloat(void) const
        {
            return float( );
        }

        ////////////////////////////////////////////////////////////////////////////

        template<typename T>
        double VariantContainer<T>::ToDouble(void) const
        {
            return double( );
        }

        ////////////////////////////////////////////////////////////////////////////

        template<typename T>
        std::string VariantContainer<T>::ToString(void) const
        {
            return std::string( );
        }

        ////////////////////////////////////////////////////////////////////////////

        template<typename T>
        void *VariantContainer<T>::GetPtr(void) const
        {
            return static_cast<void*>( m_value );
        }

        ////////////////////////////////////////////////////////////////////////////

        template<typename T>
        VariantBase *VariantContainer<T>::Clone(void) const
        {
            return new VariantContainer<T>( *m_value );
        }
    }
}
