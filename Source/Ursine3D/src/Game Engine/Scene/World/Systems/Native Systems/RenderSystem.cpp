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

#include "WorldConfigComponent.h"
#include "SelectedComponent.h"
#include "Model3DComponent.h"
#include "LightComponent.h"
#include "Billboard2DComponent.h"
#include "CameraComponent.h"
#include "ParticleSystemComponent.h"
#include "AnimatorComponent.h"

#include "GfxAPI.h"
#include "RenderableComponentBase.h"

namespace ursine
{
    namespace ecs
    {
        ENTITY_SYSTEM_DEFINITION( RenderSystem );

        RenderSystem::RenderSystem(World *world)
            : EntitySystem( world )
            , m_worldConfig( nullptr )
        {
            m_graphics = GetCoreSystem( graphics::GfxAPI );
        }

        void RenderSystem::SortCameraArray(void)
        {
            utils::InsertionSort( m_cameras, cameraSortPredicate );
        }

        RenderSystem::~RenderSystem(void)
        {

        }

        void RenderSystem::OnInitialize(void)
        {
            m_world->Listener( this )
                .On( WORLD_UPDATE, &RenderSystem::onUpdate )
                .On( WORLD_RENDER, &RenderSystem::onRender )
                .On( WORLD_EDITOR_UPDATE, &RenderSystem::onEditorUpdate )
                .On( WORLD_EDITOR_RENDER, &RenderSystem::onEditorRender )
                .On( WORLD_ENTITY_COMPONENT_ADDED, &RenderSystem::onComponentAdded )
                .On( WORLD_ENTITY_COMPONENT_REMOVED, &RenderSystem::onComponentRemoved );
        }

        void RenderSystem::OnRemove(void)
        {
            m_world->Listener( this )
                .Off( WORLD_UPDATE, &RenderSystem::onUpdate )
                .Off( WORLD_RENDER, &RenderSystem::onRender )
                .Off( WORLD_EDITOR_UPDATE, &RenderSystem::onEditorUpdate )
                .Off( WORLD_EDITOR_RENDER, &RenderSystem::onEditorRender )
                .Off( WORLD_ENTITY_COMPONENT_ADDED, &RenderSystem::onComponentAdded )
                .Off( WORLD_ENTITY_COMPONENT_REMOVED, &RenderSystem::onComponentRemoved );
        }

        void RenderSystem::OnAfterLoad(void)
        {
            m_worldConfig = m_world->GetSettings( )->GetComponent<WorldConfig>( );
        }

        bool RenderSystem::cameraSortPredicate(Component::Handle<Camera> a, Component::Handle<Camera> b)
        {
            return a->GetSortLayer( ) <= b->GetSortLayer( );
        }

        void RenderSystem::onComponentAdded(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, ComponentEventArgs);

            auto *component = const_cast<Component*>( args->component );

            if (component->Is<Camera>( ))
            {
                utils::InsertionSort(
                    m_cameras,
                    static_cast<Camera*>( component ),
                    cameraSortPredicate
                );
            }
            else if (component->Is<Model3D>( ))
            {
                addRenderable( args->entity, static_cast<Model3D*>( component )->m_base );
            }
            else if (component->Is<Billboard2D>( ))
            {
                addRenderable( args->entity, static_cast<Billboard2D*>( component )->m_base );
            }
            else if (component->Is<Light>( ))
            {
                addRenderable( args->entity, static_cast<Light*>( component )->m_base );
            }
            else if (component->Is<Animator>( ))
            {
                m_animators.emplace_back(
                    static_cast<Animator*>( component )
                );
            }
            else if (component->Is<ParticleSystem>( ))
            {
                addRenderable( args->entity, static_cast<ParticleSystem*>( component )->m_base );
            }
        }

        void RenderSystem::onComponentRemoved(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, ComponentEventArgs);

            auto *component = const_cast<Component*>( args->component );

            if (component->Is<Camera>( ))
            {
                URSINE_TODO("Replace this with a utils::BinarySearch call");
                auto search = std::find(
                    m_cameras.begin( ),
                    m_cameras.end( ),
                    static_cast<Camera*>( component )
                );

                if (search != m_cameras.end( ))
                    m_cameras.erase( search );
            }
            else if (component->Is<Model3D>( ))
                removeRenderable( args->entity, static_cast<Model3D*>( component )->m_base );
            else if (component->Is<Billboard2D>( ))
                removeRenderable( args->entity, static_cast<Billboard2D*>( component )->m_base );
            else if (component->Is<Light>( ))
                removeRenderable( args->entity, static_cast<Light*>( component )->m_base );
            else if (component->Is<Animator>( ))
            {
                auto search = std::find(
                    m_animators.begin( ),
                    m_animators.end( ),
                    static_cast<Animator*>( component )
                );

                if (search != m_animators.end( ))
                    m_animators.erase( search );
            }
            else if (component->Is<ParticleSystem>( ))
            {
                removeRenderable( args->entity, static_cast<ParticleSystem*>( component )->m_base );
            }
        }

        void RenderSystem::onUpdate(EVENT_HANDLER(World))
        {
            auto dt = Application::Instance->GetDeltaTime( );

            for (auto &animator : m_animators)
                animator->UpdateAnimation( dt );
        }

        void RenderSystem::onRender(EVENT_HANDLER(World))
        {
            m_graphics->BeginScene( );

            renderObjects( );

            RenderHookArgs e( 0 );

            for (size_t i = 0; i < m_cameras.size( ); ++i)
            {
                auto camera = m_cameras[ i ];

                if (!camera->GetActive( ))
                    continue;

                renderCamera( camera, e, RENDER_HOOK );
            }

            m_graphics->EndScene( );
        }

        void RenderSystem::onEditorUpdate(EVENT_HANDLER(World))
        {
            auto dt = Application::Instance->GetDeltaTime( );

            for (auto &animator : m_animators)
            {
                // update only if selected
                if (animator.GetEntity( )->HasComponent<Selected>( ))
                    animator->UpdateAnimation( dt );
            }
        }

        void RenderSystem::onEditorRender(EVENT_HANDLER(World))
        {
            m_graphics->BeginScene( );

            renderObjects( );

            RenderHookArgs e( 0 );

            for (size_t i = 0; i < m_cameras.size( ); ++i)
            {
                auto camera = m_cameras[ i ];

                auto isEditorCamera = camera->IsEditorCamera( );
                auto isSelected = camera.GetEntity( )->HasComponent<Selected>( );

                // skip non editor cameras and non selected cameras
                if (isEditorCamera || isSelected)
                {
                    renderCamera( camera, e, RENDER_HOOK_EDITOR );
                }
            }

            m_graphics->EndScene( );
        }

        void RenderSystem::renderObjects(void)
        {
            for (auto &mapPair : m_renderableMap)
            {
                auto &renderableVec = mapPair.second;

                for (auto &rend : renderableVec)
                {
                    if (rend->m_dirty)
                        rend->m_updateRenderer( );

                    m_graphics->RenderObject( rend->m_handle );
                }
            }
        }

        void RenderSystem::renderCamera(Component::Handle<Camera> camera, RenderHookArgs &args, RenderSystemEventType hook)
        {
            if (camera->m_dirty)
                camera->updateRenderer( );

            args.camera = camera->m_handle;

            Dispatch( hook, &args );

            m_graphics->RenderScene( 0.0f, camera->m_handle );
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
