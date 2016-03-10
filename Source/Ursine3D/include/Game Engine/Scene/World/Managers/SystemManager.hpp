/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SystemManager.hpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

namespace ursine
{
    namespace ecs
    {
        template<class SystemType>
        bool SystemManager::HasSystem(void)
        {
            static_assert( std::is_base_of<EntitySystem, SystemType>::value,
                "The type is not a system!" );

            return m_systems.size( ) > SystemType::SystemID && 
                   m_systems[ SystemType::SystemID ] != nullptr;
        }

        template<class SystemType>
        inline SystemType *SystemManager::GetSystem(void)
        {
            static_assert( std::is_base_of<EntitySystem, SystemType>::value,
                "The type is not a system!" );

            return static_cast<SystemType *>( m_systems[ SystemType::SystemID ] );
        }

        template<class SystemType>
        SystemType *SystemManager::AddSystem(void)
        {
            static_assert( std::is_base_of<EntitySystem, SystemType>::value,
                "The type is not a system!" );

            auto systemType = typeof( SystemType );

            UAssert( !HasSystem<SystemType>( ),
                "System type '%s' already exists.",
                systemType.GetName( ).c_str( )
            );

            auto systemID = systemType.GetStaticField( "SystemID" )
                .GetValue( )
                .GetValue<SystemTypeID>( );
                
            UAssert( systemID != -1, 
                "System ID for type '%s' has not been initialized.\n"
                "Possibly forgot ENTITY_SYSTEM_DEFINITION."
            );

            // systems take a pointer to a world as the first argument
            const auto systemConstructor = 
                meta::InvokableSignature { typeof( World* ) };

            auto constructor = 
                systemType.GetDynamicConstructor( systemConstructor );

            UAssert( constructor.IsValid( ), 
                "System type missing dynamic constructor %s(World *)",
                systemType.GetName( ).c_str( )
            );

            auto *system = 
                constructor.Invoke( m_world ).GetValue<EntitySystem*>( );

            system->m_typeID = systemID;

            m_systems[ systemID ] = system;

            if (m_initialized)
                system->OnInitialize( );

            auto *scene = m_world->GetOwner( );

            if (scene != nullptr)
                system->OnSceneReady( scene );

            return static_cast<SystemType*>( system );
        }
    }
}
