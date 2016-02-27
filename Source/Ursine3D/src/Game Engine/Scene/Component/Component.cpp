#include "UrsinePrecompiled.h"

#include "Component.h"

#include "Scene.h"

namespace ursine
{
    namespace ecs
    {
        Component::~Component(void)
        {
            if (m_hasResources)
                GetOwner( )->GetWorld( )->Disconnect( WORLD_OWNER_CHANGED, this, &Component::onSceneChanged );
        }

        void Component::initialize(void)
        {
            static const auto resourceRefType = typeof( resources::ResourceReference );

            auto type = GetType( );
            auto fields = type.GetFields( );

        #if defined(URSINE_WITH_EDITOR)
            auto instance = ObjectVariant( this );
        #endif

            for (auto &field : fields)
            {
                auto fieldType = field.GetType( );

            #if defined(URSINE_WITH_EDITOR)
                // array types
                if (fieldType.IsArray( ))
                {
                    auto fieldInstance = field.GetValue( instance );

                    auto &events = fieldInstance.GetArray( ).GetModifyEvents( );

                    auto onModification = [=](EVENT_HANDLER(meta::Variant))
                    {
                        EVENT_ATTRS(meta::Variant, ArrayModificationArgs);

                        auto *owner = GetOwner( );
                        
                        if (owner)
                        {
                            EditorComponentArrayModfiedArgs e( *args, owner, this, field.GetName( ) );

                            owner->GetWorld( )->Dispatch( WORLD_EDITOR_COMPONENT_ARRAY_MODIFIED, &e );
                        }
                    };

                    // we can get away with not disconnecting this because the events have the same
                    // scope as the component itself
                    events.Connect<EventArgs>( AMODIFY_INSERT, onModification );
                    events.Connect<EventArgs>( AMODIFY_SET, onModification );
                    events.Connect<EventArgs>( AMODIFY_REMOVE, onModification );
                }
                // resource reference types
                else 
            #endif
                if (!m_hasResources && fieldType == resourceRefType)
                {
                    m_hasResources = true;

                #if defined(URSINE_WITH_EDITOR)
                    auto &reference = field
                        .GetValueReference( instance )
                        .GetValue<resources::ResourceReference>( );

                    // notify changed automatically
                    auto onReferenceChanged = [=](EVENT_HANDLER(resources::ResourceReference))
                    {
                        EVENT_ATTRS(resources::ResourceReference, EventArgs);

                        auto *scene = GetOwner( )->GetWorld( )->GetOwner( );

                        if (scene)
                            sender->m_manager = &scene->GetResourceManager( );

                        auto ref = meta::Variant { *sender, meta::variant_policy::NoCopy( ) };

                        NOTIFY_COMPONENT_CHANGED( field.GetName( ), ref );
                    };

                    reference.Connect<EventArgs>( resources::RR_REFERENCED_RESOURCE_CHANGED, onReferenceChanged );
                #else
                    break;
                #endif
                }
            }


        #if defined(URSINE_WITH_EDITOR)
            m_baseInitialized = true;
        #endif

            if (m_hasResources)
                GetOwner( )->GetWorld( )->Connect( WORLD_OWNER_CHANGED, this, &Component::onSceneChanged );

            OnInitialize( );
        }

        void Component::onSceneChanged(EVENT_HANDLER(World))
        {
            static const auto resourceRefType = typeof( resources::ResourceReference );

            auto *scene = GetOwner( )->GetWorld( )->GetOwner( );

            UAssert( scene != nullptr,
                "Scene was null."  
            );

            auto *resourceManager = &scene->GetResourceManager( );

            auto type = GetType( );
            auto fields = type.GetFields( );

            auto instance = ObjectVariant( this );

            for (auto &field : fields)
            {
                auto fieldType = field.GetType( );

                if (field.GetType( ) == resourceRefType)
                {
                    auto &reference = field
                        .GetValueReference( instance )
                        .GetValue<resources::ResourceReference>( );

                    reference.setManager( resourceManager );
                }
            }
        }

        template<>
        Transform *Component::Handle<Transform>::operator->(void)
        {
            return m_entity->GetTransform( );
        }

        template<>
        const Transform *Component::Handle<Transform>::operator->(void) const
        {
            return m_entity->GetTransform( );
        }

    }
}
