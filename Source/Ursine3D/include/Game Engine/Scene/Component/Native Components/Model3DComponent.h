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

namespace ursine
{
    namespace ecs
    {
        class Model3D : public Component
        {
            NATIVE_COMPONENT;

            friend class RenderSystem;

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
                Color color,
                GetColor,
                SetColor
            );

            EditorField(
                std::string modelName,
                GetModelResourceName,
                SetModelResourceName
            );

            EditorField(
                std::string materialName,
                GetMaterial,
                SetMaterial
            );

            EditorMeta(BitMaskEditor)
            EditorField(
                ursine::ecs::RenderMask renderMask,
                GetRenderMask,
                SetRenderMask
            );

            EditorField(
                float emissive,
                GetEmissive,
                SetEmissive
            );

            EditorField(
                float specularPower,
                GetSpecularPower,
                SetSpecularPower
            );

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

            Meta(Enable)
            Model3D(void);
            ~Model3D(void);

            void OnInitialize(void) override;

            std::vector<SMat4> &GetMatrixPalette(void);

            //get/set model name
            void SetModelResourceName(const std::string &name);
            const std::string &GetModelResourceName(void) const;

            // get the model resource to access the mesh
            const graphics::ModelResource *GetModelResource(void) const;

            void SetMaterial(const std::string &name);
            const std::string &GetMaterial(void) const;

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

            ursine::ecs::RenderMask GetRenderMask(void) const;
            void SetRenderMask(ursine::ecs::RenderMask mask);

            void SetMaterialData(float emiss, float pow, float intensity);
            void GetMaterialData(float &emiss, float &pow, float &intensity);

            void SetMeshIndex(const int index);
            int GetMeshIndex(void) const;

            bool GetIsShadowCaster(void) const;
            void SetIsShadowCaster(bool castShadows);

            void OnSerialize(Json::object &output) const override;
            void OnDeserialize(const Json &input) override;

        private:

            // This model component's model in the renderer
            graphics::Model3D *m_model;

            RenderableComponentBase *m_base;

            std::string m_modelName;
            std::string m_materialName;

            void updateRenderer(void);

        } Meta(Enable, WhiteListMethods, DisplayName( "Model3D" ));
    }
}