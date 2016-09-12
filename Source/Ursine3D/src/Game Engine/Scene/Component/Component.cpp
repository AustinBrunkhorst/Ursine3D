/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** Component.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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

            m_hasResources = false;

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
                else if (!m_hasResources && fieldType == typeof( ursine::resources::ResourceReference ))
                {
                    m_hasResources = true;
                }
            }

        #endif

            OnInitialize( );
        }

        void Component::onSceneReady(Scene *scene)
        {
        #if defined(URSINE_WITH_EDITOR)

            if (m_hasResources)
            {
                scene->Connect( 
                    SCENE_RESOURCE_MODIFIED,
                    this, 
                    &Component::onResourceModifed 
                );

                m_resourceEventDestructors.emplace_back( [=] {
                    scene->Disconnect( SCENE_RESOURCE_MODIFIED, this, &Component::onResourceModifed );
                } );
            }

        #endif

            OnSceneReady( scene );
        }

        #if defined(URSINE_WITH_EDITOR)

        void Component::onBeforeRemove(void)
        {
            for (auto &dtor : m_resourceEventDestructors)
                dtor( );

            m_resourceEventDestructors.clear( );
        }

        void Component::onResourceModifed(EVENT_HANDLER(Scene))
        {
            EVENT_ATTRS(Scene, SceneResourceModifiedArgs);

            if (!m_owner)
                return;

            auto search = m_resourceModificationCallbacks.find( args->resourceGUID );

            if (search != m_resourceModificationCallbacks.end( ))
                search->second( );
        }

        #endif

        template<>
        Transform *Component::Handle<Transform>::operator->(void) const
        {
            return m_entity->GetTransform( );
        }
    }
}
