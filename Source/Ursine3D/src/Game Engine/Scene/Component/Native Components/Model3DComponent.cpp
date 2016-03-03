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
#include "ConvexDecompColliderComponent.h"
#include "BvhTriangleMeshColliderComponent.h"
#include "Notification.h"
#include "ModelResource.h"
#include "LvlHierarchy.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION(Model3D);

        Model3D::Model3D(void)
            : BaseComponent( )
            , m_graphics( GetCoreSystem( graphics::GfxAPI ) )
            , m_model( nullptr )
        {
            m_base = new RenderableComponentBase( std::bind( &Model3D::updateRenderer, this ) );

            m_base->SetHandle( m_graphics->RenderableMgr.AddRenderable( graphics::RENDERABLE_MODEL3D ) );

            // store a pointer to the model
            m_model = &m_graphics->RenderableMgr.GetModel3D( m_base->GetHandle( ) );

            m_model->SetRenderMask( 0 );
        }

        Model3D::~Model3D(void)
        {
            m_base->OnRemove( GetOwner( ) );

            m_model->SetDebug(false);

           m_graphics->RenderableMgr.DestroyRenderable( m_base->GetHandle( ) );

            // release resource - need to call unload model, texture.
            m_graphics->ResourceMgr.UnloadModel( m_model->GetModelHandle( ) );
            m_graphics->ResourceMgr.UnloadTexture( m_model->GetTextureHandle( ) );

            delete m_base;
        }

        void Model3D::OnInitialize(void)
        {
            auto &owner = GetOwner( );

            m_base->OnInitialize(owner);

            m_model->SetEntityID( GetOwner( )->GetID( ) );

            updateRenderer( );
        }

        void Model3D::OnSceneReady(Scene *scene)
        {
            invalidateModel( false );
            invalidateTexture( false );
        }

        std::vector<SMat4> &Model3D::GetMatrixPalette(void)
        {
            return m_model->GetMatrixPalette();
        }

        const resources::ResourceReference& Model3D::GetModel(void) const
        {
            return m_modelResource;
        }

        void Model3D::SetModel(const resources::ResourceReference &model)
        {
            m_modelResource = model;

            if (!resourcesAreAvailable( ))
                return;

            invalidateModel( );

            NOTIFY_COMPONENT_CHANGED( "model", m_modelResource );
        }

        const resources::ResourceReference& Model3D::GetTexture(void) const
        {
            return m_textureResource;
        }

        void Model3D::SetTexture(const resources::ResourceReference &texture)
        {
            m_textureResource = texture;

            if (!resourcesAreAvailable( ))
                return;

            invalidateTexture( );

            NOTIFY_COMPONENT_CHANGED( "texture", m_textureResource );
        }

        const graphics::ModelResource *Model3D::GetModelResource(void) const
        {
            return m_graphics->ResourceMgr.GetModelResource( m_model->GetModelHandle( ) );
        }

        void Model3D::SetColor(const ursine::Color &color)
        {
            m_model->SetColor(color);

            NOTIFY_COMPONENT_CHANGED("color", color);
        }

        const Color &Model3D::GetColor(void)
        {
            return m_model->GetColor();
        }

        float Model3D::GetEmissive(void) const
        {
            return m_model->GetEmissive();
        }

        void Model3D::SetEmissive(float emissive)
        {
            m_model->SetEmissive(emissive);
        }

        float Model3D::GetSpecularPower(void) const
        {
            return m_model->GetSpecularPower();
        }

        void Model3D::SetSpecularPower(float power)
        {
            m_model->SetSpecularPower(power);
        }

        float Model3D::GetSpecularIntensity(void) const
        {
            return m_model->GetSpecularIntensity();
        }

        void Model3D::SetSpecularIntensity(float intensity)
        {
            m_model->SetSpecularIntensity(intensity);
        }

        void Model3D::SetOverdraw(bool flag)
        {
            m_model->SetOverdraw(flag);
        }

        bool Model3D::GetOverdraw(void) const
        {
            return m_model->GetOverdraw();
        }

        void Model3D::SetDebug(bool flag)
        {
            m_model->SetDebug(flag);
        }

        bool Model3D::GetDebug(void) const
        {
            return m_model->GetDebug();
        }

        RenderMask Model3D::GetRenderMask(void) const
        {
            return static_cast<RenderMask>(m_model->GetRenderMask() & 0xFFFFFFFF);
        }

        void Model3D::SetRenderMask(RenderMask mask)
        {
            m_model->SetRenderMask(static_cast<unsigned long long>(mask));
        }

        void Model3D::SetMaterialData(float emiss, float pow, float intensity)
        {
            m_model->SetMaterialData(emiss, pow, intensity);
        }

        void Model3D::GetMaterialData(float &emiss, float &pow, float &intensity)
        {
            m_model->GetMaterialData(emiss, pow, intensity);
        }

        void Model3D::SetMeshIndex(const int index)
        {
            m_model->SetMeshIndex(index);
        }

        int Model3D::GetMeshIndex(void) const
        {
            return m_model->GetMeshIndex();
        }

        bool Model3D::GetIsShadowCaster(void) const
        {
            return m_model->GetShadowCaster();
        }

        void Model3D::SetIsShadowCaster(bool castShadows)
        {
            m_model->SetShaderCaster(castShadows);
        }

        void Model3D::updateRenderer(void)
        {
            auto trans = GetOwner( )->GetTransform( );
            auto &model = m_graphics->RenderableMgr.GetModel3D( m_base->GetHandle( ) );

            model.SetWorldMatrix(trans->GetLocalToWorldMatrix());
        }

        void Model3D::invalidateModel(bool unload)
        {
            auto data = loadResource<resources::ModelData>( m_modelResource );

            if (data == nullptr)
            {
                // default
                m_model->SetModelHandle( 0 );
            }
            else
            {
                auto handle = data->GetModelHandle( );

                if (unload)
                    m_graphics->ResourceMgr.UnloadModel( m_model->GetModelHandle( ) );

                m_graphics->ResourceMgr.LoadModel( handle );

                m_model->SetModelHandle( handle );
            }
        }

        void Model3D::invalidateTexture(bool unload)
        {
            auto data = loadResource<resources::TextureData>( m_textureResource );

            if (data == nullptr)
            {
                // default
                m_model->SetTextureHandle( 1 );
            }
            else
            {
                auto handle = data->GetTextureHandle();

                if (unload)
                    m_graphics->ResourceMgr.UnloadTexture( m_model->GetTextureHandle( ) );

                m_graphics->ResourceMgr.LoadTexture( handle );

                m_model->SetTextureHandle(handle);
            }
        }

        void Model3D::OnSerialize(Json::object &output) const
        {
            output["meshIndex"] = GetMeshIndex();
        }

        void Model3D::OnDeserialize(const Json &input)
        {
            SetMeshIndex(input["meshIndex"].int_value());
        }

    #if defined(URSINE_WITH_EDITOR)

        void Model3D::GenerateConvexHull(void)
        {
            auto entity = GetOwner();

            Application::PostMainThread( [=]
            {
                if (!entity->HasComponent<ConvexHullCollider>())
                    entity->AddComponent<ConvexHullCollider>();

                auto convexHull = entity->GetComponent<ConvexHullCollider>();

                convexHull->GenerateConvexHull(this);
            });
        }

        void Model3D::GenerateBvhTriangleMeshCollider(void)
        {
            auto entity = GetOwner();

            Application::PostMainThread( [=]
            {
                if (!entity->HasComponent<BvhTriangleMeshCollider>())
                    entity->AddComponent<BvhTriangleMeshCollider>();

                auto bvhTriangleMesh = entity->GetComponent<BvhTriangleMeshCollider>();

                bvhTriangleMesh->GenerateBvhTriangleMesh(this);
            });

            // Send notification of collider's limitations
            NotificationConfig config;

            config.type = NOTIFY_INFO;
            config.dismissible = true;
            config.duration = TimeSpan::FromSeconds(15.0f);
            config.header = "BVH Triangle Mesh Collider Limitations";
            config.message = "<ol><li>Performance intensive.<li/>"
                "<li>Cannot be Dynamic.<li/>"
                "<li>Need Dynamic concave colliders? Use <strong>Convex Decomposition<strong>.<li/><ol/>";

            EditorPostNotification(config);
        }

        void Model3D::GenerateConvexDecompCollider(void)
        {
            auto entity = GetOwner();

            Application::PostMainThread( [=]
            {
                if (!entity->HasComponent<ConvexDecompCollider>())
                    entity->AddComponent<ConvexDecompCollider>();

                auto convex = entity->GetComponent<ConvexDecompCollider>();

                convex->GenerateConvexHulls(this);
            });
        }

    #endif
    }
}
