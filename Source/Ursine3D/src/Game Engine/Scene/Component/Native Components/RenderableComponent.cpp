#include "UrsinePrecompiled.h"

#include "RenderableComponent.h"
#include "EntityEvent.h"
#include "GfxAPI.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( Renderable );

        Renderable::Renderable(void)
            : BaseComponent( )
            , m_handle( 0 )
        {
            
        }

        Renderable::~Renderable(void)
        {
            GetOwner( )->Listener( this )
                .Off( ENTITY_TRANSFORM_DIRTY, &Renderable::onTransformChange );

            GetCoreSystem(graphics::GfxAPI)->RenderableMgr.DestroyRenderable(m_handle);
        }

        graphics::GfxHND Renderable::GetHandle(void) const
        {
            return m_handle;
        }

        void Renderable::SetHandle(graphics::GfxHND handle)
        {
            m_handle = handle;
        }

        void Renderable::OnInitialize(void)
        {
            GetOwner( )->Listener( this )
                .On( ENTITY_TRANSFORM_DIRTY, &Renderable::onTransformChange );
        }

        void Renderable::onTransformChange(EVENT_HANDLER(Entity))
        {
            m_dirty = true;
        }

        void Renderable::updateRenderer(void)
        {
            GetOwner( )->Dispatch( ENTITY_UPDATE_RENDERER, EventArgs::Empty );

            m_dirty = false;
        }
    }
}