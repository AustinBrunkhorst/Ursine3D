/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RenderSystem.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "RenderSystem.h"

#include "Model3DComponent.h"
#include "LightComponent.h"
#include "Billboard2DComponent.h"
#include "CameraComponent.h"
#include "FBXSceneRootNodeComponent.h"

#include "GfxAPI.h"

#include <mutex>

namespace ursine
{
    namespace ecs
    {
        ENTITY_SYSTEM_DEFINITION( RenderSystem );

        RenderSystem::RenderSystem(World *world)
            : EntitySystem( world )
        {
            m_graphics = GetCoreSystem( graphics::GfxAPI );
        }

        void RenderSystem::SortCameraArray()
        {
            utils::InsertionSort( m_cameras, cameraSortPredicate );
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

        bool RenderSystem::cameraSortPredicate(ursine::ecs::Camera* a, ursine::ecs::Camera* b)
        {
            return a->GetRenderLayer( ) <= b->GetRenderLayer( );
        }

        void RenderSystem::onComponentAdded(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, ComponentEventArgs);

            if (args->component->Is<Camera>( ))
            {
                utils::InsertionSort(
                    m_cameras, 
                    static_cast<Camera*>( const_cast<Component*>( args->component ) ), 
                    cameraSortPredicate 
                );
            }

            else if (args->component->Is<Model3D>( ))
                addRenderable( args->entity, static_cast<Model3D*>( const_cast<Component*>( args->component ) ) );
            else if (args->component->Is<Billboard2D>( ))
                addRenderable( args->entity, static_cast<Billboard2D*>( const_cast<Component*>( args->component ) ) );
            else if (args->component->Is<Light>( ))
                addRenderable( args->entity, static_cast<Light*>( const_cast<Component*>( args->component ) ) );
            else if ( args->component->Is<Animator>( ) )
            {
                m_animators.emplace_back(
					static_cast<Animator*>(const_cast<Component*>(args->component))
                );
            }
        }

        void RenderSystem::onComponentRemoved(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, ComponentEventArgs);

            if (args->component->Is<Camera>( ))
            {
                URSINE_TODO("Replace this with a utils::BinarySearch call");
                auto search = std::find(
                    m_cameras.begin( ), 
                    m_cameras.end( ), 
                    static_cast<Camera*>( const_cast<Component*>( args->component ) )
                );

                if (search != m_cameras.end( ))
                    m_cameras.erase( search );
            }
            else if (args->component->Is<Model3D>( ))
                removeRenderable( args->entity, static_cast<Model3D*>( const_cast<Component*>( args->component ) ) );
            else if (args->component->Is<Billboard2D>( ))
                removeRenderable( args->entity, static_cast<Billboard2D*>( const_cast<Component*>( args->component ) ) );
            else if (args->component->Is<Light>( ))
                removeRenderable( args->entity, static_cast<Light*>( const_cast<Component*>( args->component ) ) );
			else if (args->component->Is<Animator>( ))
			{
				auto search = std::find(
					m_animators.begin(),
					m_animators.end(),
					static_cast<Animator*>(const_cast<Component*>(args->component))
				);

				if (search != m_animators.end( ))
					m_animators.erase( search );
			}
        }

        void RenderSystem::onRender(EVENT_HANDLER(World))
        {
            m_graphics->BeginScene( );

            for (auto &animator : m_animators)
                animator->UpdateAnimation( Application::Instance->GetDeltaTime( ) );

            for (auto &mapPair : m_renderableMap)
            {
                auto &renderableVec = mapPair.second;
                
                for (auto &rend : renderableVec)
                {
                    if (rend->m_dirty)
                        rend->updateRenderer( );

                    m_graphics->RenderObject( rend->m_handle );
                }
            }

            RenderHookArgs e( 0 );

			for (size_t i = 0; i < m_cameras.size( ); ++i)
            {
				auto *camera = m_cameras[ i ];

                if (camera->m_dirty)
                    camera->updateRenderer( );

                if (!camera->GetActive( ))
                    continue;

                e.camera = camera->m_handle;

                Dispatch( RENDER_HOOK, &e );

                m_graphics->RenderScene( 0.0f, camera->m_handle );
            }

            m_graphics->EndScene( );
        }

        void RenderSystem::addRenderable(Entity *entity, RenderableComponentBase *renderable)
        {
            m_renderableMap[ entity->GetUniqueID( ) ].push_back( renderable );
        }

        void RenderSystem::removeRenderable(Entity *entity, RenderableComponentBase *renderable)
        {
            // See if we have a renderable vector for this entity
            auto id = entity->GetUniqueID( );
            auto vecSearch = m_renderableMap.find( id );

            if (vecSearch != m_renderableMap.end( ))
            {
                // If so, see if we have this component's base class in the vector
                auto &renderableVec = m_renderableMap[ id ];
                auto renderableSearch = std::find( renderableVec.begin( ), renderableVec.end( ), renderable );

                // If so remove it
                if (renderableSearch != renderableVec.end( ))
                    renderableVec.erase( renderableSearch );

                // If vector is empty, remove it from the map
                if (renderableVec.size( ) == 0)
                    m_renderableMap.erase( id );
            }
        }
    }
}