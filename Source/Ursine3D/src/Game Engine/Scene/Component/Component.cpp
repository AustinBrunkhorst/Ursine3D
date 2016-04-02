#include "UrsinePrecompiled.h"

#include "Component.h"

#include "Scene.h"

namespace ursine
{
    namespace ecs
    {
        Component::~Component(void)
        {
        #if defined(URSINE_WITH_EDITOR)

            m_owner->GetWorld( )->Disconnect( WORLD_EDITOR_RESOURCE_MODIFIED, this, &Component::onResourceModifed );

        #endif
        }

        bool Component::resourcesAreAvailable(void) const
        {
            return m_owner->GetWorld( )->GetOwner( ) != nullptr;
        }

        void Component::onInitialize(void)
        {
        #if defined(URSINE_WITH_EDITOR)
            auto type = GetType( );
            auto fields = type.GetFields( );

            auto instance = ObjectVariant( this );

            for (auto &field : fields)
            {
                auto fieldType = field.GetType( );

                // array types
                if (fieldType.IsArray( ))
                {
                    auto fieldInstance = field.GetValue( instance );

                    auto &events = fieldInstance.GetArray( ).GetModifyEvents( );

                    auto onModification = [=](EVENT_HANDLER(meta::Variant))
                    {
                        EVENT_ATTRS(meta::Variant, ArrayModificationArgs);

                        auto &owner = GetOwner( );
                        
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
            }

            m_owner->GetWorld( )->Connect( WORLD_EDITOR_RESOURCE_MODIFIED, this, &Component::onResourceModifed );
        #endif

            OnInitialize( );
        }

        void Component::onSceneReady(Scene *scene)
        {
            OnSceneReady( scene );
        }

        #if defined(URSINE_WITH_EDITOR)

        void Component::onResourceModifed(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, EditorWorldResourceModifiedArgs);

            if (!m_owner)
                return;

            auto search = m_resourceModificationCallbacks.find( args->resourceGUID );

            if (search != m_resourceModificationCallbacks.end( ))
                search->second( );
        }

        #endif

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
