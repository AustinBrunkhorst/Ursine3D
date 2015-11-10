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
        NATIVE_COMPONENT_DEFINITION( Model3D );

        Model3D::Model3D(void)
            : BaseComponent( )
            , m_model( nullptr )
        {
            auto *graphics = GetCoreSystem( graphics::GfxAPI );

            m_handle = graphics->RenderableMgr.AddRenderable( graphics::RENDERABLE_MODEL3D );

            // store a pointer to the model
            m_model = &graphics->RenderableMgr.GetModel3D( m_handle );
        }

        Model3D::~Model3D(void)
        {
            // Unsubscribe from the Renderable component's event
            GetOwner( )->Listener( this )
                .Off( ENTITY_UPDATE_RENDERER, &Model3D::onUpdateRenderer );
        }

        void Model3D::OnInitialize(void)
        {
            auto *owner = GetOwner( );

            // Subscribe from the Renderable component's event
            owner->Listener( this )
                .On( ENTITY_UPDATE_RENDERER, &Model3D::onUpdateRenderer );

            // Set the handle in the renderable component
            if (!owner->HasComponent<Renderable>( ))
                owner->AddComponent<Renderable>( );

            owner->GetComponent<Renderable>( )->SetHandle( m_handle );

            // set the unique id
            m_model->SetEntityUniqueID( GetOwner( )->GetUniqueID( ) );

            // set the model if there is one
            if (m_modelName.size( ) > 0)
                m_model->SetModel( m_modelName );

            if (m_materialName.size( ) > 0)
                m_model->SetMaterial( m_materialName );

            onUpdateRenderer( this, EventArgs::Empty );
        }

        void Model3D::SetModel(const std::string &name)
        {
            m_modelName = name;

            m_model->SetModel( name );

            // Default
            m_model->SetMaterial( "Cube" );
        }

        const std::string &Model3D::GetModel(void) const
        {
            return m_modelName;
        }

        void Model3D::SetMaterial(const std::string &name)
        {
            m_materialName = name;

            m_model->SetMaterial( name );
        }

        const std::string &Model3D::GetMaterial(void) const
        {
            return m_materialName;
        }

        void Model3D::SetColor(const ursine::Color &color)
        {
            m_model->SetColor( color );

            NOTIFY_COMPONENT_CHANGED( "color", color );
        }

        const Color &Model3D::GetColor()
        {
            return m_model->GetColor( );
        }

        void Model3D::SetDebug(bool flag)
        {
            m_model->SetDebug( flag );
        }

        bool Model3D::GetDebug() const
        {
            return m_model->GetDebug( );
        }

        void Model3D::SetMaterialData(float emiss, float pow, float intensity)
        {
            m_model->SetMaterialData( emiss, pow, intensity );
        }

        void Model3D::GetMaterialData(float& emiss, float& pow, float& intensity)
        {
            m_model->GetMaterialData( emiss, pow, intensity );
        }

        void Model3D::onUpdateRenderer(EVENT_HANDLER(Entity))
        {
            // update the renderer's
            auto ren = GetOwner( )->GetComponent<Renderable>( );
            auto trans = GetOwner( )->GetTransform( );
            auto handle = ren->GetHandle( );
            auto &model = GetCoreSystem( graphics::GfxAPI )->RenderableMgr.GetModel3D( handle );

            model.SetWorldMatrix( trans->GetLocalToWorldMatrix( ) );
        }
    }
}
