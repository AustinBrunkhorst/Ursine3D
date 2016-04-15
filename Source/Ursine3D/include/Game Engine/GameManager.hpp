namespace ursine
{
    template<typename T>
    ecs::Component *GameManager::GetConfigComponent(void)
    {
        auto search = m_configComponents.find( typeof( T ) );

        return search == m_configComponents.end( ) ? nullptr : search->second;
    }
}