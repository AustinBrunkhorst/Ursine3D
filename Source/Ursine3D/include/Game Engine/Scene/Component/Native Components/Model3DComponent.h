#pragma once

#include "Component.h"
#include "Renderable.h"
#include "GfxAPI.h"

namespace ursine
{
    namespace ecs
    {
        class Model3D : public Component
        {
            NATIVE_COMPONENT;

        public:
            EditorField(
                Color color,
                GetColor,
                SetColor
            );

            EditorField(
                std::string modelName,
                GetModel,
                SetModel
            );

            EditorField(
                std::string materialName,
                GetMaterial,
                SetMaterial
            );

            Model3D(void);
            ~Model3D(void);

            Meta(Disable)
            void OnInitialize(void) override;

            //get/set model
            void SetModel(const std::string &name);
            const std::string &GetModel(void) const;
            
            void SetMaterial(const std::string &name);
            const std::string &GetMaterial(void) const;

            //get/set color
            void SetColor(const ursine::Color &color);
            const ursine::Color &GetColor(void);

            // turn debug mode on or off for this model
            void SetDebug(bool flag);
            bool GetDebug(void) const;

            void SetMaterialData(float emiss, float pow, float intensity);
            void GetMaterialData(float &emiss, float &pow, float &intensity);

        private:
            graphics::GfxHND m_handle;

            // This model component's model in the renderer
            graphics::Model3D *m_model;

            std::string m_modelName;
            std::string m_materialName;

            void onUpdateRenderer(EVENT_HANDLER(Entity));

        } Meta(Enable, DisplayName( "Model3D" ));
    }
}