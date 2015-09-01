/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RenderSystem.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "RenderSystem.h"
#include "RenderableComponent.h"

#include "RenderEffects.h"

#include "Entity.h"

#include "Utils.h"

namespace ursine
{
    namespace ecs
    {
        // we need a low priority because we want to update last
        RenderSystem::RenderSystem(World *world)
            : EntitySystem(world, -1000)
        {
            
        }

        RenderSystem::~RenderSystem(void)
        {
            
        }

        void RenderSystem::OnInitialize(void)
        {
            _world->Manager<EntityManager>()->Listener(this)
                .On(EM_ENTITY_ADDED, &RenderSystem::onEntityAdded)
                .On(EM_ENTITY_REMOVED, &RenderSystem::onEntityRemoved);

            _world->Manager<SystemManager>()->Listener(this)
                .On(SM_DRAW, &RenderSystem::onDraw);
        }

        void RenderSystem::OnRemove(void)
        {
            _world->Manager<EntityManager>()->Listener(this)
                .Off(EM_ENTITY_ADDED, &RenderSystem::onEntityAdded)
                .Off(EM_ENTITY_REMOVED, &RenderSystem::onEntityRemoved);

            _world->Manager<SystemManager>()->Listener(this)
                .Off(SM_DRAW, &RenderSystem::onDraw);
        }

        void RenderSystem::OnLayerChanged(Entity *owner, RenderLayer layer_old, RenderLayer layer_new)
        {
            auto effect = owner->GetComponent<Renderable>()->GetEffect( );

            auto &container_old = _renderable[ layer_old ][ effect ];
            auto &container_new = _renderable[ layer_new ][ effect ];

            auto lower = lower_bound(
                container_old.begin( ), 
                container_old.end( ), 
                owner, 
                compareTexture);

            container_old.erase(
                find( lower, container_old.end( ), owner ));

            utils::InsertionSort( container_new, owner, compareTexture );
        }

        void RenderSystem::OnTextureChanged(Entity *owner, Renderable *component)
        {
            auto &container = _renderable
                [ component->GetLayer() ]
                [ component->GetEffect( ) ];

            URSINE_TODO("fix when shaders implemented");
            sort( container.begin(), container.end(), compareTexture);
        }

        void RenderSystem::onEntityAdded(EVENT_HANDLER(EntityManager))
        {
            EVENT_ATTRS(EntityManager, EntityEventArgs);

            if (args->entity->HasComponent<Renderable>())
            {
                auto renderable = args->entity->GetComponent<Renderable>();

                auto &container = _renderable
                    [renderable->GetLayer( )]
                    [renderable->GetEffect( )];

                utils::InsertionSort( container, args->entity, compareTexture );
            }
        }

        void RenderSystem::onEntityRemoved(EVENT_HANDLER(EntityManager))
        {
            EVENT_ATTRS(EntityManager, EntityEventArgs);

            auto entity = args->entity;

            auto renderable = entity->GetComponent<Renderable>( );

            if (renderable)
            {
                auto &container = _renderable
                    [ renderable->GetLayer( ) ]
                    [ renderable->GetEffect( ) ];

                auto lower = lower_bound(
                    container.begin( ), 
                    container.end( ), 
                    entity, 
                    compareTexture);

                container.erase(
                    find(lower, container.end( ), entity));
            }
        }

        void RenderSystem::onDraw(EVENT_HANDLER(SystemManager))
        {
            for (auto &layer : _renderable)
            {
                auto &effects = layer.second;

                for (uint32 i = 0; i < effects.size( ); ++i)
                {
                    auto effect = GetRenderEffect( static_cast<RenderEffectType>(i) );

                    effect->Bind( _world );

                    for (auto entity : effects[ i ])
                    {
                        effect->Render( entity->GetComponent<Renderable>( ) );
                    }

                    effect->UnBind( );
                }
            }
        }

        bool RenderSystem::compareTexture(const Entity *a, const Entity *b)
        {
            auto texture_a = a->GetComponent<Renderable>( )->GetTexture( );
            auto texture_b = b->GetComponent<Renderable>( )->GetTexture( );

            if (!texture_a || !texture_b)
                return false;

            return texture_a->GetID( ) < texture_b->GetID( );
        }
    }
}
