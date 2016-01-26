#include "UrsinePrecompiled.h"

#include "Component.h"

namespace ursine
{
    namespace ecs
    {
        void Component::Initialize(void)
        {
        #if defined(URSINE_WITH_EDITOR)

            auto type = GetType( );
            auto fields = type.GetFields( );

            auto instance = meta::Variant { this, meta::variant_policy::WrapObject( ) };

            for (auto &field : fields)
            {
                if (field.GetType( ).IsArray( ))
                {
                    auto fieldInstance = field.GetValue( instance );

                    auto &events = fieldInstance.GetArray( ).GetModifyEvents( );

                    auto onModification = [=](EVENT_HANDLER(meta::Variant))
                    {
                        EVENT_ATTRS(meta::Variant, ArrayModificationArgs);

                        auto *owner = GetOwner( );
                        
                        if (owner)
                        {
                            EditorComponentArrayModfiedArgs e( *args, field.GetName( ) );

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

            m_baseInitialized = true;

        #endif

            OnInitialize( );
        }

        void Component::OnInitialize(void) { }

    } // ecs namespece

}  // ursine namespace