#include "UrsinePrecompiled.h"

#include "Component.h"

#include "Scene.h"

namespace ursine
{
    namespace ecs
    {
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
        #endif

            OnInitialize( );
        }

        void Component::onSceneReady(Scene *scene)
        {
            OnSceneReady( scene );
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
