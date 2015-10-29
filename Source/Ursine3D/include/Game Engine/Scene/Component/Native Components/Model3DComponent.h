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
                Color Color,
                GetColor,
                SetColor
                );

        public:
            Model3D(void);
            ~Model3D(void);

            Meta(Disable)
            void OnInitialize(void) override;

            //get/set model
            void SetModel(const std::string &name);
            ursine::graphics::Model3D *GetModel(void);
            
            //get/set color
            void SetColor(const ursine::Color &color);
            const ursine::Color &GetColor(void);

        private:

            // This model component's model in the renderer
            graphics::Model3D *m_model;

            // The graphics core API
            graphics::GfxAPI *m_graphics;

            void onUpdateRenderer(EVENT_HANDLER(Entity));

        } Meta( Enable, DisplayName( "Model3D" ) );
    }
}