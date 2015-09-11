namespace ursine
{
    template<typename SystemType>
    inline SystemType *Application::GetCoreSystem(void)
    {
        static_assert( std::is_base_of<core::CoreSystem, SystemType>::value,
            "The type is not a system!" );

        return static_cast<SystemType *>( m_systems[ SystemType::SystemID ] );
    }
}