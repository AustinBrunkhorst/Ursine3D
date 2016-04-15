#include "UrsinePrecompiled.h"

#include "GameManager.h"

#include "WorldSerializer.h"
#include "WorldData.h"

namespace ursine
{
    GameManager::GameManager(void)
        : m_context( nullptr ) { }

    GameManager::~GameManager(void)
    {
        for (auto &entry : m_configComponents)
            delete entry.second;

        m_configComponents.clear( );
    }

    GameContext *GameManager::GetContext(void)
    {
        return m_context;
    }

    void GameManager::OnInitialize(GameContext *context, const ursine::Json &configObj)
    {
        m_context = context;

        static const auto gameManagerPtr = typeof( GameManager* );

        for (auto &entry : configObj.object_items( ))
        {
            auto configType = meta::Type::GetFromName( entry.first );

        #if defined(URSINE_WITH_EDITOR)

            if (!configType.IsValid( ))
            {
                UWarning( "Unknown config type '%s'.", entry.first.c_str( ) );

                continue;
            }

        #else

            UAssert( configType.IsValid( ),
                "Unknown config type '%s'.",
                entry.first.c_str( )
            );

        #endif
            

            auto ctor = configType.GetDynamicConstructor( { gameManagerPtr } );

        #if defined(URSINE_WITH_EDITOR)

            if (!ctor.IsValid( ))
            {
                UWarning( 
                    "Config type '%s' missing constructor %s(%s)",
                    configType.GetName( ).c_str( ),
                    configType.GetName( ).c_str( ),
                    gameManagerPtr.GetName( ).c_str( )
                );

                continue;
            }

        #else

            UAssert( ctor.IsValid( ),
                "Config type '%s' missing constructor %s(%s)",
                configType.GetName( ).c_str( ),
                configType.GetName( ).c_str( ),
                gameManagerPtr.GetName( ).c_str( )
            );

        #endif

            auto *config = ctor.Invoke( context ).GetValue<ecs::Component*>( );
            auto instance = ObjectVariant( config );

            instance.GetType( ).DeserializeJson( instance, entry.second );

            m_configComponents[ configType ] = config;
        }
    }

    ecs::World *GameManager::setWorld(const resources::ResourceReference &resource, bool loadConfiguredSystems/* = true*/)
    {
        auto *scene = m_context->GetScene( );

        auto *data = resource.Load<resources::WorldData>( scene->GetResourceManager( ) );

        if (!data)
            return nullptr;

        auto json = ecs::WorldSerializer::Serialize( data->GetData( ).get( ) );

        auto *world = ecs::WorldSerializer::Deserialize( json );

        scene->SetActiveWorld( ecs::World::Handle( world ) );

        if (loadConfiguredSystems)
            scene->LoadConfiguredSystems( );

        return world;
    }
}