/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SystemManager.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "SystemManager.h"

namespace ursine
{
    namespace ecs
    {
        void configureSystems(void)
        {
            static bool configured = false;

            if (configured)
                return;

            configured = true;

            auto systemType = typeof( EntitySystem );

            SystemTypeID nextID = 0;

            for (auto derived : systemType.GetDerivedClasses( ))
            {
                auto systemID = derived.GetStaticField( "SystemID" );

                UAssert( systemID.IsValid( ),
                    "Entity system '%s' doesn't have a static field SystemID.\n"
                    "Most likely missing ENTITY_SYSTEM in declaration",
                    derived.GetName( ).c_str( )
                );

                systemID.SetValue( nextID++ );
            }
        }

        SystemManager::SystemManager(World *world)
            : WorldManager( world )
        {
            configureSystems( );

            auto systemType = typeof( EntitySystem );

            auto &systemTypes = systemType.GetDerivedClasses( );

            m_systems.resize( systemTypes.size( ) );

            // systems take a pointer to a world as the first argument
            const auto systemConstructor = 
                meta::InvokableSignature { typeof( World* ) };

            for (auto derived : systemType.GetDerivedClasses( ))
            {
                auto systemID = derived.GetStaticField( "SystemID" )
                    .GetValue( )
                    .GetValue<SystemTypeID>( );
                
                UAssert( systemID != -1, 
                    "System ID for type '%s' has not been initialized.\n"
                    "Possibly forgot ENTITY_SYSTEM_DEFINITION."
                );

                auto constructor = 
                    derived.GetDynamicConstructor( systemConstructor );

                UAssert( constructor.IsValid( ), 
                    "System type missing dynamic constructor %s(World *)",
                    derived.GetName( ).c_str( )
                );

                auto *system = 
                    constructor.Invoke( world ).GetValue<EntitySystem*>( );

                system->m_typeID = systemID;

                m_systems[ systemID ] = system;
            }
        }

        SystemManager::~SystemManager(void)
        {
            for (auto *system : m_systems)
                system->OnRemove( );

            for (auto *system : m_systems)
                delete system;
        }

        void SystemManager::OnInitialize(void)
        {
            for (auto system : m_systems)
                system->OnInitialize( );
        }
    }
}
