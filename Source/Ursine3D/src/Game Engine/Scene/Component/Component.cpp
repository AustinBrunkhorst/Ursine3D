#include "UrsinePrecompiled.h"

#include "Component.h"

namespace ursine
{
    namespace ecs
    {
    #if defined(URSINE_WITH_EDITOR)

        void Component::initArrayEvents(Component *component)
        {
            auto type = component->GetType( );
            auto fields = type.GetFields( );

            auto instance = meta::Variant { component, meta::variant_policy::WrapObject( ) };

            for (auto &field : fields)
            {
                if (field.GetType( ).IsArray( ))
                {
                    auto fieldInstance = field.GetValue( instance );

                    auto &events = fieldInstance.GetArray( ).GetModifyEvents( );

                    // we can get away with not disconnecting this because the events have the same
                    // scope as the component itself
                    //events.Connect( AMODIFY_INSERT, events::bind( &Component::onArrayModified, component, 5 ) );

                    //.On( AMODIFY_SET, &Component::onArrayModified )
                    //.On( AMODIFY_REMOVE, &Component::onArrayModified );
                }
            }
        }

        void onArrayModified(EVENT_HANDLER(meta::Variant))
        {
            EVENT_ATTRS(meta::Variant, ArrayModificationArgs);
        }

    #endif
    }
}