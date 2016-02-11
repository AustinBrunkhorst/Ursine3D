/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RenderableComponentBase.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "RenderableComponentBase.h"
#include "EntityEvent.h"

namespace ursine
{
    RenderableComponentBase::RenderableComponentBase(std::function<void(void)> UpdateRenderer)
        : m_handle( 0 )
		, m_updateRenderer( UpdateRenderer ) { }

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

	graphics::GfxHND &RenderableComponentBase::GetHandle(void)
	{
		return m_handle;
	}

	void RenderableComponentBase::SetHandle(const graphics::GfxHND& handle)
	{
		m_handle = handle;
	}
}
