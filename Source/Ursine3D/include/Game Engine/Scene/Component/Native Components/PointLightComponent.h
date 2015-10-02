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
        #if defined(__REFLECTION_PARSER__)

            Meta(Getter( "GetColor" ), Setter( "SetColor" ))
            Color color;

            Meta(Getter( "GetRadius" ), Setter( "SetRadius" ))
            float radius;

        #endif

            PointLight(void);
            ~PointLight(void);

            Meta(Disable)
            void OnInitialize(void) override;

            //get/set color
            const Color &GetColor(void) const;
            void SetColor(const Color &color);

            //get/set position
            void SetPosition(const SVec3 &position);

            //get/set radius
            float GetRadius(void) const;
            void SetRadius(double radius);

        private:
            ursine::PointLight *m_pointLight;

            friend class RenderSystem;
        } Meta( Enable, DisplayName( "PointLight" ) );
    }
}