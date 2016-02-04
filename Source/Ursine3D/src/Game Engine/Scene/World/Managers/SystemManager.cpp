/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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
        const auto &getSystemTypes(void)
        {
            static bool configured = false;

            // types of base system types
            static meta::Type::List baseTypes
            {
                typeof( EntitySystem ),
                typeof( FilterSystem )
            };

            static meta::Type::List systemTypes;

            if (!configured)
            {
                for (auto systemType : baseTypes)
                {
                    for (auto derived : systemType.GetDerivedClasses( ))
                    {
                        auto search = find(
							baseTypes.begin( ),
							baseTypes.end( ),
                            derived
                        );

                        // skip base clases of base system types
                        if (search == baseTypes.end( ))
                            systemTypes.emplace_back( derived );
                    }
                }
            }

            configured = true;

            return systemTypes;
        }

        void configureSystems(void)
        {
            static bool configured = false;

            if (configured)
                return;

            configured = true;

            auto &systemTypes = getSystemTypes( );

            SystemTypeID nextID = 0;

            for (auto systemType : systemTypes)
            {
                auto systemID = systemType.GetStaticField( "SystemID" );

                UAssert( systemID.IsValid( ),
                    "Entity system '%s' doesn't have a static field SystemID.\n"
                    "Most likely missing ENTITY_SYSTEM in declaration",
                    systemType.GetName( ).c_str( )
                );

                systemID.SetValue( nextID++ );
            }
        }

        const meta::Type::List &SystemManager::GetExposedTypes(void)
        {
            return getSystemTypes( );
        }

        SystemManager::SystemManager(World *world)
            : WorldManager( world )
            , m_initialized( false )
            , m_loaded( false )
        {
            configureSystems( );

            auto &systemTypes = getSystemTypes( );

            m_systems.resize( systemTypes.size( ) );

            // systems take a pointer to a world as the first argument
            const auto systemConstructor = 
                meta::InvokableSignature { typeof( World* ) };

            for (auto systemType : systemTypes)
            {
                auto systemID = systemType.GetStaticField( "SystemID" )
                    .GetValue( )
                    .GetValue<SystemTypeID>( );
                
                UAssert( systemID != -1, 
                    "System ID for type '%s' has not been initialized.\n"
                    "Possibly forgot ENTITY_SYSTEM_DEFINITION."
                );

                auto &systemMeta = systemType.GetMeta( );

                // disabling auto add to the world
                if (systemMeta.GetProperty<DisableEntitySystemAutoAdd>( ))
                {
                    m_systems[ systemID ] = nullptr;

                    continue;
                }

                auto &constructor = 
                    systemType.GetDynamicConstructor( systemConstructor );

                UAssert( constructor.IsValid( ), 
                    "System type missing dynamic constructor %s(World *)",
                    systemType.GetName( ).c_str( )
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
            {
                if (system)
                    system->OnRemove( );
            }
                
            for (auto *system : m_systems)
            {
                if (system)
                    delete system;
            }
        }

        void SystemManager::OnInitialize(void)
        {
            for (auto system : m_systems)
            {   
                if (system)
                    system->OnInitialize( );
            }
                
            m_initialized = true;
        }

        void SystemManager::onAfterLoad(void)
        {
            for (auto system : m_systems)
            {
                if (system)
                    system->OnAfterLoad( );
            }

            m_loaded = true;
        }
    }
}
