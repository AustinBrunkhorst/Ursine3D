#include "UrsinePrecompiled.h"
#include "Billboard2DComponent.h"
#include "RenderableComponent.h"
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

        } 

        Billboard2D::~Billboard2D(void)
        {
            GetOwner()->Listener(this)
                .Off(ENTITY_UPDATE_RENDERER, &Billboard2D::onUpdateRenderer);
        }

        void Billboard2D::OnInitialize() 
        {
            GetOwner()->Listener(this)
                .On(ENTITY_UPDATE_RENDERER, &Billboard2D::onUpdateRenderer);

            // store a pointer to the GfxAPI core system
            m_graphics = GetCoreSystem(graphics::GfxAPI);

            auto handle = m_graphics->RenderableMgr.AddRenderable(graphics::RENDERABLE_BILLBOARD2D);
            GetOwner()->GetComponent<Renderable>()->SetHandle(handle);

            // store a pointer to the model
            m_billboard = &m_graphics->RenderableMgr.GetBillboard2D(handle);

            // set the unique id
            m_billboard->SetEntityUniqueID(GetOwner()->GetUniqueID());
        }

        void Billboard2D::onUpdateRenderer(EVENT_HANDLER(Entity)) 
        {
            auto ren = GetOwner()->GetComponent<Renderable>();
            auto trans = GetOwner()->GetTransform();
            auto handle = ren->GetHandle();
            auto &billboard = GetCoreSystem(graphics::GfxAPI)->RenderableMgr.GetBillboard2D(handle);

            billboard.SetPosition(trans->GetWorldPosition());
        }

        ursine::graphics::Billboard2D* Billboard2D::GetBillboard()
        {
            return m_billboard;
        }
    }
}