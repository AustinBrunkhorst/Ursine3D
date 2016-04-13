/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Model3DComponent.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "Renderable.h"
#include "RenderableComponentBase.h"
#include "RenderMask.h"
#include "ModelResource.h"

#include "TextureData.h"
#include "ModelData.h"

namespace ursine
{
    namespace graphics
    {
        class GfxAPI;
    }

    namespace ecs
    {
        class Model3D : public Component
        {
            NATIVE_COMPONENT;

            friend class RenderSystem;
            friend class ModelFragmenter;

        public:
            EditorButton(
                GenerateConvexHull,
                "Generate Convex Hull"
            );

            EditorButton(
                GenerateBvhTriangleMeshCollider,
                "Generate BVH Triangle Mesh Collider"
            );

            EditorButton(
                GenerateConvexDecompCollider,
                "Generate Convex Decomposition Collider"
            );

            EditorField(
                bool active,
                GetActive,
                SetActive
            );

            EditorField(
                bool overdraw,
                GetDoesOverdraw,
                SetDoesOverdraw
            );

            EditorField(
                bool visible,
                GetIsVisible,
                SetIsVisible
            );

            EditorField(
                bool warping,
                GetDoesWarp,
                SetDoesWarp
            );

            EditorField(
                Color color,
                GetColor,
                SetColor
            );

            EditorResourceField(
                ursine::resources::ModelData,
                model,
                GetModel,
                SetModel
            );

            EditorResourceField(
                ursine::resources::TextureData,
                texture,
                GetTexture,
                SetTexture
            );

            EditorResourceField(
                ursine::resources::TextureData,
                normalMap,
                GetNormalTexture,
                SetNormalTexture
            );

            EditorMeta(BitMaskEditor)
            EditorField(
                ursine::ecs::RenderMask renderMask,
                GetRenderMask,
                SetRenderMask
            );

            EditorMeta(InputRange(0.0f, 2.0f, 0.01f, "{{value.toPrecision( 2 )}}"))
            EditorField(
                float emissive,
                GetEmissive,
                SetEmissive
            );

            EditorMeta(InputRange(0.0f, 255.0f, 1.0f, "{{value.toPrecision( 3 )}}"))
            EditorField(
                float specularPower,
                GetSpecularPower,
                SetSpecularPower
            );

            EditorMeta(InputRange(0.0f, 1.0f, 0.01f, "{{value.toPrecision( 2 )}}"))
            EditorField(
                float specularIntensity,
                GetSpecularIntensity,
                SetSpecularIntensity
            );

            EditorField(
                bool castsShadows,
                GetIsShadowCaster,
                SetIsShadowCaster
            );

            EditorField(
                Vec2 diffuseTextureOffset,
                GetTextureUVOffset,
                SetTextureUVOffset
            );

            EditorField(
                Vec2 emissiveTextureOffset,
                GetEmissiveTextureUVOffset,
                SetEmissiveTextureUVOffset
            );

            Meta(Enable)
            Model3D(void);
            ~Model3D(void);

            void OnInitialize(void) override;
            void OnSceneReady(Scene *scene) override;

            const resources::ResourceReference &GetModel(void) const;
            void SetModel(const resources::ResourceReference &model);

            const resources::ResourceReference &GetTexture(void) const;
            void SetTexture(const resources::ResourceReference &texture);

            const resources::ResourceReference &GetNormalTexture(void) const;
            void SetNormalTexture(const resources::ResourceReference &texture);

            // get the model resource to access the mesh
            const graphics::ModelResource *GetModelResource(void) const;

            //get/set color
            void SetColor(const ursine::Color &color);
            const ursine::Color &GetColor(void);

            //get/set material properties
            float GetEmissive(void) const;
            void SetEmissive(float emissive);

            float GetSpecularPower(void) const;
            void SetSpecularPower(float power);

            float GetSpecularIntensity(void) const;
            void SetSpecularIntensity(float intesnity);

            void SetOverdraw(bool flag);
            bool GetOverdraw(void) const;

            // turn debug mode on or off for this model
            void SetDebug(bool flag);
            bool GetDebug(void) const;

            void SetActive(bool flag);
            bool GetActive(void) const;

            ursine::ecs::RenderMask GetRenderMask(void) const;
            void SetRenderMask(ursine::ecs::RenderMask mask);

            void SetMaterialData(float emiss, float pow, float intensity);
            void GetMaterialData(float &emiss, float &pow, float &intensity);

            void SetMeshIndex(const int index);
            int GetMeshIndex(void) const;

            bool GetIsShadowCaster(void) const;
            void SetIsShadowCaster(bool castShadows);

            const Vec2 &GetTextureUVOffset(void) const;
            void SetTextureUVOffset(const Vec2 &offset);

            const Vec2 &GetTextureUVScalar(void) const;
            void SetTextureUVScalar(const Vec2 &scalar);

            const Vec2 &GetEmissiveTextureUVOffset(void) const;
            void SetEmissiveTextureUVOffset(const Vec2 &offset);

            const Vec2 &GetEmissiveTextureUVScalar(void) const;
            void SetEmissiveTextureUVScalar(const Vec2 &scalar);

            bool GetIsVisible(void) const;
            void SetIsVisible(bool isVisible);

            bool GetDoesOverdraw(void);
            void SetDoesOverdraw(bool doesOverdraw);

            bool GetDoesWarp(void) const;
            void SetDoesWarp(bool doesWarp);

            void OnSerialize(Json::object &output) const override;
            void OnDeserialize(const Json &input) override;

            std::string GetModelName(void) const;

        private:
            friend class AnimatorSystem;

            graphics::GfxAPI *m_graphics;

            // This model component's model in the renderer
            graphics::Model3D *m_model;

            RenderableComponentBase *m_base;
            
            resources::ResourceReference m_modelResource;
            resources::ResourceReference m_textureResource;
            resources::ResourceReference m_normalTextureResource;

            void updateRenderer(void);

            void invalidateTexture(bool unload = true);
            void invalidateNormalTexture(bool unload = true);
            void invalidateModel(bool unload = true);

            void onTextureReload(void);
            void onNormalTextureReload(void);
            void OnModelReload(void);
            void clearMatrixPalette(void);

            std::vector<SMat4> &getMatrixPalette(void);
            std::vector<SMat4> &getMatrixPaletteIT(void);
        } Meta(Enable, WhiteListMethods, DisplayName( "Model3D" ));
    }
}