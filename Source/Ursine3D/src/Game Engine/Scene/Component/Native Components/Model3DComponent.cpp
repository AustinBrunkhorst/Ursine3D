/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Model3DComponent.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Model3DComponent.h"
#include "CoreSystem.h"
#include "GfxAPI.h"
#include "ConvexHullColliderComponent.h"
#include "BvhTriangleMeshColliderComponent.h"
#include "Notification.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( Model3D );

        Model3D::Model3D(void)
            : BaseComponent( )
            , m_model( nullptr )
			, m_modelName( "Cube" )
			, m_materialName( "Blank" )
        {
            auto *graphics = GetCoreSystem( graphics::GfxAPI );

            m_handle = graphics->RenderableMgr.AddRenderable( graphics::RENDERABLE_MODEL3D );

            // store a pointer to the model
            m_model = &graphics->RenderableMgr.GetModel3D( m_handle );
        }

        Model3D::~Model3D(void)
        {
            RenderableComponentBase::OnRemove( GetOwner( ) );

            m_model->SetDebug( false );

            GetCoreSystem( graphics::GfxAPI )->RenderableMgr.DestroyRenderable( m_handle );
        }

        void Model3D::OnInitialize(void)
        {
            Component::OnInitialize( );

            auto *owner = GetOwner( );

            RenderableComponentBase::OnInitialize( owner );

            // set the unique id
            m_model->SetEntityUniqueID( GetOwner( )->GetUniqueID( ) );

            // set the model if there is one
            if (m_modelName.size( ) > 0)
                m_model->SetModel( m_modelName );

            if (m_materialName.size( ) > 0)
                m_model->SetMaterial( m_materialName );

            updateRenderer( );
        }

        std::vector<SMat4>& Model3D::GetMatrixPalette()
        {
            return m_model->GetMatrixPalette( );
        }

        void Model3D::SetModelResourceName(const std::string &name)
        {
            m_modelName = name;

            m_model->SetModel( name );

            // Default
            m_model->SetMaterial( "Cube" );
        }

        const std::string &Model3D::GetModelResourceName(void) const
        {
            return m_modelName;
        }

        const graphics::ModelResource *Model3D::GetModelResource(void) const
        {
            return GetCoreSystem(graphics::GfxAPI)->ResourceMgr.GetModelResource( m_modelName );
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

        void Model3D::SetOverdraw(bool flag)
        {
            m_model->SetOverdraw( flag );
        }

        bool Model3D::GetOverdraw(void) const
        {
            return m_model->GetOverdraw( );
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

        int Model3D::GetRenderMask() const
        {
            int retVal = static_cast<int>( m_model->GetRenderMask( ) & 0xFFFFFFFF );

            return retVal;
        }

        void Model3D::SetRenderMask(const int mask)
        {
            m_model->SetRenderMask( static_cast<unsigned long long>(mask) );
        }

        void Model3D::SetMaterialData(float emiss, float pow, float intensity)
        {
            m_model->SetMaterialData( emiss, pow, intensity );
        }

        void Model3D::GetMaterialData(float& emiss, float& pow, float& intensity)
        {
            m_model->GetMaterialData( emiss, pow, intensity );
        }

        void Model3D::SetMeshIndex(const int index)
        {
            m_model->SetMeshIndex( index );
        }

	    int Model3D::GetMeshIndex(void) const
	    {
			return m_model->GetMeshIndex( );
	    }

	    void Model3D::updateRenderer(void)
        {
            // update the renderer's
            auto trans = GetOwner( )->GetTransform( );
            auto &model = GetCoreSystem( graphics::GfxAPI )->RenderableMgr.GetModel3D( m_handle );

            model.SetWorldMatrix( trans->GetLocalToWorldMatrix( ) );
        }

	    void Model3D::OnSerialize(Json::object &output) const
	    {
			output[ "meshIndex" ] = GetMeshIndex( );
	    }

	    void Model3D::OnDeserialize(const Json &input)
	    {
			SetMeshIndex( input[ "meshIndex" ].int_value( ) );
	    }

    #if defined(URSINE_WITH_EDITOR)

        void Model3D::GenerateConvexHull(void)
        {
			auto entity = GetOwner( );

            if (!entity->HasComponent<ConvexHullCollider>( ))
				entity->AddComponent<ConvexHullCollider>( );

			auto convexHull = entity->GetComponent<ConvexHullCollider>( );

			convexHull->GenerateConvexHull( this );
        }

		void Model3D::GenerateBvhTriangleMeshCollider(void)
        {
			auto entity = GetOwner( );

	        Timer::Create(0).Completed([=] {
				if (!entity->HasComponent<BvhTriangleMeshCollider>( ))
					entity->AddComponent<BvhTriangleMeshCollider>( );

				auto bvhTriangleMesh = entity->GetComponent<BvhTriangleMeshCollider>( );

				bvhTriangleMesh->GenerateBvhTriangleMesh( this );
			} );

			// Send notification of collider's limitations
			NotificationConfig config;

			config.type = NOTIFY_INFO;
			config.dismissible = true;
			config.duration = TimeSpan::FromSeconds( 15.0f );
			config.header = "BVH Triangle Mesh Collider Limitations";
			config.message = "The BVH Triangle Mesh Collider has limitations and performance implications."
							 " 1. Dynamic rigidbodies using this collider will not function properly."
							 " 2. Scaling this collider during runtime has been optimized, but may still cause performance issues."
							 " 3. Please also consider \"Convex Decomposition\", this is useful if a dynamic mesh is required.";

			EditorPostNotification( config );
        }

    #endif
    }
}
