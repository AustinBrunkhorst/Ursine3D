/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Billboard2DComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "Billboard2DComponent.h"
#include "CoreSystem.h"
#include "GfxAPI.h"
#include "EntityEvent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION(Billboard2D);

        Billboard2D::Billboard2D(void) 
            : BaseComponent()
            , m_billboard(nullptr)
        {
            // store a pointer to the GfxAPI core system
            m_graphics = GetCoreSystem(graphics::GfxAPI);

			m_base = new RenderableComponentBase([=] {
				updateRenderer( );
			});

            m_base->SetHandle( m_graphics->RenderableMgr.AddRenderable(graphics::RENDERABLE_BILLBOARD2D) );

            // store a pointer to the model
            m_billboard = &m_graphics->RenderableMgr.GetBillboard2D(m_base->GetHandle( ));
        } 

        Billboard2D::~Billboard2D(void)
        {
            m_base->OnRemove( GetOwner( ) );

            m_graphics->RenderableMgr.DestroyRenderable( m_base->GetHandle( ) );

			delete m_base;
        }

        void Billboard2D::OnInitialize() 
        {
            m_base->OnInitialize( GetOwner( ) );

            // set the unique id
            m_billboard->SetEntityUniqueID(GetOwner()->GetUniqueID());
        }

        void Billboard2D::updateRenderer(void)
        {
            auto trans = GetOwner()->GetTransform();
            auto &billboard = GetCoreSystem(graphics::GfxAPI)->RenderableMgr.GetBillboard2D( m_base->GetHandle( ) );

            billboard.SetPosition(trans->GetWorldPosition());
        }

        ursine::graphics::Billboard2D* Billboard2D::GetBillboard()
        {
            return m_billboard;
        }
    }
}