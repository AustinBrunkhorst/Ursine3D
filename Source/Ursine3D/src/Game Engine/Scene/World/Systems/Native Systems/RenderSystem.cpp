#include "UrsinePrecompiled.h"

#include "RenderSystem.h"

#include "RenderableComponent.h"
#include "CameraComponent.h"

#include "GfxAPI.h"

namespace ursine
{
    namespace ecs
    {
        ENTITY_SYSTEM_DEFINITION( RenderSystem );

        RenderSystem::RenderSystem(World *world)
            : EntitySystem( world )
        {
            m_graphics = GetCoreSystem(graphics::GfxAPI );
        }

        RenderSystem::~RenderSystem(void)
        {
            
        }

        void RenderSystem::OnInitialize(void)
        {
            m_world->Listener( this )
                .On( WORLD_RENDER, &RenderSystem::onRender )
                .On( WORLD_ENTITY_COMPONENT_ADDED, &RenderSystem::onComponentAdded )
                .On( WORLD_ENTITY_COMPONENT_REMOVED, &RenderSystem::onComponentRemoved );
        }

        void RenderSystem::OnRemove(void)
        {
            m_world->Listener( this )
                .Off( WORLD_RENDER, &RenderSystem::onRender )
                .Off( WORLD_ENTITY_COMPONENT_ADDED, &RenderSystem::onComponentAdded )
                .Off( WORLD_ENTITY_COMPONENT_REMOVED, &RenderSystem::onComponentRemoved );
        }

        void RenderSystem::onComponentAdded(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, ComponentEventArgs);

            if (args->component->Is<Camera>( ))
            {
                m_cameras.emplace( 
                    args->entity->GetUniqueID( ), 
                    static_cast<Camera*>( const_cast<Component*>( args->component ) )
                );
            }
            else if (args->component->Is<Renderable>( ))
            {
                m_renderable.emplace( 
                    args->entity->GetUniqueID( ), 
                    static_cast<Renderable*>( const_cast<Component*>( args->component ) )
                );
            }
        }

        void RenderSystem::onComponentRemoved(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, ComponentEventArgs);

            if (args->component->Is<Camera>( ))
            {
                auto search = m_cameras.find( args->entity->GetUniqueID( ) );

                if (search != m_cameras.end( ))
                    m_cameras.erase( search );
            }
            else if (args->component->Is<Renderable>( ))
            {
                auto search = m_renderable.find( args->entity->GetUniqueID( ) );

                if (search != m_renderable.end( ))
                    m_renderable.erase( search );
            }
        }

        void RenderSystem::onRender(EVENT_HANDLER(World))
        {
            m_graphics->BeginScene( );

            for (auto &renderable : m_renderable)
                m_graphics->RenderObject( renderable.second->m_handle );

            RenderHookArgs e( 0 );

            for (auto &camera : m_cameras) 
            {
                e.camera = camera.second->m_handle;

                Dispatch( RENDER_HOOK, &e );

                m_graphics->RenderScene( 0.0f, camera.second->m_handle );
            }

            m_graphics->EndScene( );
        }
    }
}