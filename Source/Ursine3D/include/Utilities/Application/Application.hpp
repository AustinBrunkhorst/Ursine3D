namespace ursine
{
    template<typename SystemType>
    inline SystemType *Application::CoreSystem(void)
    {
        static_assert( std::is_base_of<core::CoreSystem, SystemType>::value,
            "The type is not a system!" );

    #if defined(CONFIG_DEBUG)

        UAssert( SystemType::SystemID != static_cast<unsigned>( -1 ),
            "Unknown core system '%s'. Is it enabled in reflection?",
            typeid( SystemType ).name( )
        );
        
    #endif

        return static_cast<SystemType *>( m_systems[ SystemType::SystemID ] );
    }
}