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

            m_handle = m_graphics->RenderableMgr.AddRenderable(graphics::RENDERABLE_BILLBOARD2D);

            // store a pointer to the model
            m_billboard = &m_graphics->RenderableMgr.GetBillboard2D(m_handle);
        } 

        Billboard2D::~Billboard2D(void)
        {
            RenderableComponentBase::OnRemove( GetOwner( ) );

            m_graphics->RenderableMgr.DestroyRenderable( m_handle );
        }

        void Billboard2D::OnInitialize() 
        {
            RenderableComponentBase::OnInitialize( GetOwner( ) );

            // set the unique id
            m_billboard->SetEntityUniqueID(GetOwner()->GetUniqueID());
        }

        void Billboard2D::updateRenderer(void)
        {
            auto trans = GetOwner()->GetTransform();
            auto &billboard = GetCoreSystem(graphics::GfxAPI)->RenderableMgr.GetBillboard2D( m_handle );

            billboard.SetPosition(trans->GetWorldPosition());
        }

        ursine::graphics::Billboard2D* Billboard2D::GetBillboard()
        {
            return m_billboard;
        }
    }
}