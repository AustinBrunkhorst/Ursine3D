namespace ursine
{
    namespace meta
    {
        template<typename T>
        ArrayWrapperContainer<T>::ArrayWrapperContainer(Array<T> &a)
            : m_array( a )
        {

        }

        template<typename T>
        Variant ArrayWrapperContainer<T>::GetValue(size_t index)
        {
            return { m_array[ index ] };
        }

        template<typename T>
        void ArrayWrapperContainer<T>::SetValue(size_t index, const Argument &value)
        {
            m_array.SetValue( index, value.GetValue<T>( ) );
        }

        template<typename T>
        void ArrayWrapperContainer<T>::Insert(size_t index, const Argument &value)
        {
            m_array.Insert( index, value.GetValue<T>( ) );
        }

        template<typename T>
        void ArrayWrapperContainer<T>::Remove(size_t index)
        {
            m_array.Remove( index );
        }

        template<typename T>
        size_t ArrayWrapperContainer<T>::Size(void) const
        {
            return m_array.Size( );
        }

    #if defined(URSINE_WITH_EDITOR)

        template<typename T>
        ArrayEventDispatcher &ArrayWrapperContainer<T>::GetModifyEvents(void) const
        {
            return m_array.GetModifyEvents( );
        }

    #endif
    }
}