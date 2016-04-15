namespace ursine
{
    template<typename T>
    T *GameManager::GetConfigComponent(void)
    {
        auto search = m_configComponents.find( typeof( T ) );

        return search == m_configComponents.end( ) ? nullptr : static_cast<T*>( search->second );
    }
}