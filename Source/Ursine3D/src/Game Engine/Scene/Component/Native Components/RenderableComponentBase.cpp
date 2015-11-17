#include "UrsinePrecompiled.h"

#include "RenderableComponentBase.h"
#include "EntityEvent.h"

namespace ursine
{
    RenderableComponentBase::RenderableComponentBase(void)
        : m_handle( 0 ) { }

    void RenderableComponentBase::OnInitialize(ecs::Entity* owner)
    {
        owner->Listener( this )
            .On( ecs::ENTITY_TRANSFORM_DIRTY, &RenderableComponentBase::onTransformChange );
    }

    void RenderableComponentBase::OnRemove(ecs::Entity *owner)
    {
        owner->Listener( this )
            .Off( ecs::ENTITY_TRANSFORM_DIRTY, &RenderableComponentBase::onTransformChange );
    }

    void RenderableComponentBase::onTransformChange(EVENT_HANDLER(ecs::Entity))
    {
        m_dirty = true;
    }
}
