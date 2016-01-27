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
#include "GfxAPI.h"
#include "RenderableComponentBase.h"
#include "ModelResource.h"

namespace ursine
{
    namespace ecs
    {
        class Model3D 
            : public Component
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

            EditorField(
                int renderMask,
                GetRenderMask,
                SetRenderMask
            );

            Model3D(void);
            ~Model3D(void);

            Meta(Disable)
            void OnInitialize(void) override;

            Meta(Disable)
            std::vector<SMat4> &GetMatrixPalette( void );

            //get/set model name
            void SetModelResourceName(const std::string &name);
            const std::string &GetModelResourceName(void) const;

            // get the model resource to access the mesh
            Meta(Disable)
            const graphics::ModelResource *GetModelResource(void) const;
            
            void SetMaterial(const std::string &name);
            const std::string &GetMaterial(void) const;

            //get/set color
            void SetColor(const ursine::Color &color);
            const ursine::Color &GetColor(void);

            void SetOverdraw(bool flag);
            bool GetOverdraw(void) const;

            // turn debug mode on or off for this model
            void SetDebug(bool flag);
            bool GetDebug(void) const;

            int GetRenderMask(void) const;
            void SetRenderMask(const int mask);

            void SetMaterialData(float emiss, float pow, float intensity);
            void GetMaterialData(float &emiss, float &pow, float &intensity);

            void SetMeshIndex(const int index);
			int GetMeshIndex(void) const;

			Meta(Disable)
			void OnSerialize(Json::object &output) const override;

			Meta(Disable)
			void OnDeserialize(const Json &input) override;

        private:

            // This model component's model in the renderer
            graphics::Model3D *m_model;

			RenderableComponentBase *m_base;

            std::string m_modelName;
            std::string m_materialName;

            void updateRenderer(void);

        } Meta(Enable, DisplayName( "Model3D" ));
    }
}