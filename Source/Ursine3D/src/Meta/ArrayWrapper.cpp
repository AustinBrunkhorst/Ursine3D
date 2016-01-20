#include "UrsinePrecompiled.h"

#include "ArrayWrapper.h"

namespace ursine
{
    namespace meta
    {
        ArrayWrapper::ArrayWrapper(void)
            : m_base( nullptr )
        {

        }

        Variant ArrayWrapper::GetValue(size_t index) const
        {
            return m_base ? m_base->GetValue( index ) : nullptr;
        }

        void ArrayWrapper::SetValue(size_t index, const Argument &value)
        {
            if (m_base)
                m_base->SetValue( index, value );
        }

        void ArrayWrapper::Insert(size_t index, const Argument &value)
        {
            if (m_base)
                m_base->Insert( index, value );
        }

        void ArrayWrapper::Remove(size_t index)
        {
            if (m_base)
                m_base->Remove( index );
        }

        size_t ArrayWrapper::Size(void) const
        {
            return m_base ? m_base->Size( ) : 0;
        }

        bool ArrayWrapper::IsValid(void) const
        {
            return m_base != nullptr;
        }

    #if defined(URSINE_WITH_EDITOR)

        ArrayEventDispatcher &ArrayWrapper::GetModifyEvents(void) const
        {
            UAssert( m_base != nullptr, 
                "Invalid array." 
            );

            return m_base->GetModifyEvents( );
        }

    #endif
    }
}