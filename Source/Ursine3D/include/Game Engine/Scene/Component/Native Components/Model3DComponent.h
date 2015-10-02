#pragma once

#include "Component.h"
#include "Renderable.h"

namespace ursine
{
    namespace ecs
    {
        class Model3D : public Component
        {
            NATIVE_COMPONENT;

        public:
            Model3D(void);
            ~Model3D(void);

            Meta(Disable)
            void OnInitialize(void) override;

            //get/set model
            //get/set texture
            //get/set world matrix

        private:
            ursine::graphics::Model3D *m_model;
        } Meta( Enable, DisplayName( "Model3D" ) );
    }
}