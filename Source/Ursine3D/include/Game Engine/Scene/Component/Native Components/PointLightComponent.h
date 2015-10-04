#pragma once

#include "Component.h"
#include "Renderable.h"

#include "Renderable.h"

namespace ursine
{
    namespace ecs
    {
        class PointLight : public Component
        {
            NATIVE_COMPONENT;

        public:
            EditorField(float radius, GetRadius, SetRadius);
            EditorField(Color color, GetColor, SetColor);

            PointLight(void);
            ~PointLight(void);

            void OnInitialize(void) override;

            //get/set color
            const Color &GetColor(void) const;
            void SetColor(const Color &color);

            //get/set position
            void SetPosition(const SVec3 &position);

            //get/set radius
            float GetRadius(void) const;
            void SetRadius(float radius);

        private:
            ursine::graphics::PointLight *m_pointLight;
        } Meta(Enable, WhiteListMethods, DisplayName( "PointLight" ));
    }
}