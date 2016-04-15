#include "UrsinePrecompiled.h"

#include "GameManager.h"

namespace ursine
{
    GameManager::GameManager(GameContext *context, const ursine::Json &configObj)
        : m_context( context )
    {
        UAssert( configObj.is_object( ),
            "Expected config components to be object."
        );

        static const auto gameCtxPtrType = typeof( GameContext* );

        for (auto &entry : configObj.object_items( ))
        {
            auto configType = meta::Type::GetFromName( entry.first );

            UAssert( configType.IsValid( ),
                "Unknown config type '%s'.",
                entry.first.c_str( )
            );

            auto ctor = configType.GetDynamicConstructor( { gameCtxPtrType } );

            UAssert( ctor.IsValid( ),
                "Config type '%s' missing constructor %s(%s)",
                configType.GetName( ).c_str( ),
                configType.GetName( ).c_str( ),
                gameCtxPtrType.GetName( ).c_str( )
            );

            auto instance = ctor.Invoke( context );

            instance.GetType( ).DeserializeJson( instance, entry.second );

            auto *config = ctor.Invoke( context ).GetValue<ecs::Component*>( );

            m_configComponents[ configType ] = config;
        }
    }

    GameManager::~GameManager(void)
    {
        for (auto &entry : m_configComponents)
            delete entry.second;

        m_configComponents.clear( );
    }
}