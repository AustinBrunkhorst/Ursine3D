#include "UrsinePrecompiled.h"

#include "Model3DComponent.h"
#include "RenderableComponent.h"
#include "CoreSystem.h"
#include "GfxAPI.h"
#include "EntityEvent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION(Model3D);

        Model3D::Model3D(void)
            : BaseComponent( )
            , m_model(nullptr)
        {
        }

        Model3D::~Model3D(void)
        {
            // Unsubscribe from the Renderable component's event
            GetOwner( )->Listener( this )
                .Off( ENTITY_UPDATE_RENDERER, &Model3D::onUpdateRenderer );
        }

        void Model3D::OnInitialize(void)
        {
            // Subscribe from the Renderable component's event
            GetOwner( )->Listener( this )
                .On( ENTITY_UPDATE_RENDERER, &Model3D::onUpdateRenderer );

            // store a pointer to the GfxAPI core system
            m_graphics = GetCoreSystem( graphics::GfxAPI );

            // get ourselves a handle
            auto handle = m_graphics->RenderableMgr.AddRenderable( graphics::RENDERABLE_MODEL3D );

            // Set the handle in the renderable component
            GetOwner( )->GetComponent<Renderable>( )->SetHandle( handle );

            // store a pointer to the model
            m_model = &m_graphics->RenderableMgr.GetModel3D( handle );

            // set the unique id
            m_model->SetEntityUniqueID(GetOwner()->GetUniqueID());
        }

        void Model3D::SetModel(const std::string &name)
        {
            m_model->SetModel( name );

            // Default
            m_model->SetMaterial( "Cube" );
        }
        
        graphics::Model3D *Model3D::GetModel(void)
        {
            return m_model;
        }

        void Model3D::onUpdateRenderer(EVENT_HANDLER(Entity))
        {
            // update the renderer's
            auto ren = GetOwner( )->GetComponent<Renderable>( );
            auto trans = GetOwner( )->GetComponent<Transform>( );
            auto handle = ren->GetHandle( );
            auto &model = GetCoreSystem( graphics::GfxAPI )->RenderableMgr.GetModel3D( handle );

            model.SetWorldMatrix( trans->GetLocalToWorldMatrix( ) );
        }
    }
}
