#pragma once

#include "Component.h"
#include "Renderable.h"

namespace ursine
{
    namespace ecs
    {
        class Light : public Component
        {
            NATIVE_COMPONENT;

        public:
            EditorField(
                LightType Type,
                GetType,
                SetType
            );

            EditorField(
                SVec3 Direction,
                GetDirection,
                SetDirection
            );

            EditorField(
                Color Color,
                GetColor,
                SetColor
            );

            EditorField(
                float Intensity,
                GetIntensity,
                SetIntensity
            );

            EditorField(
                float Radius,
                GetRadius,
                SetRadius
            );

            EditorField(
                Vec2 SpotlightAngles,
                GetSpotlightAngles,
                SetSpotlightAngles
            );

        public:
            Light(void);
            ~Light(void);

            void OnInitialize(void) override;

            graphics::GfxHND GetHandle(void) const;
            const graphics::Light *GetLight(void);

            ursine::graphics::Light::LightType GetType(void);
            void SetType(const ursine::graphics::Light::LightType type);

            const SVec3 &GetDirection(void);
            void SetDirection(const SVec3 &dir);

            const SVec3 &GetPosition(void);
            void SetPosition(const SVec3 &position);

            const Color &GetColor(void);
            void SetColor(const Color &color);

            float GetRadius(void);
            void SetRadius(const float radius);

            float GetIntensity(void);
            void SetIntensity(const float intensity);

            const Vec2 &GetSpotlightAngles(void);
            void SetSpotlightAngles(const Vec2 &angles);

            void SetWorldMatrix(const Mat4 &worldTransform);

        private:
            ursine::graphics::Light *m_light;
            ursine::graphics::GfxHND m_handle;

            friend class RenderSystem;
        }Meta(Enable, DisplayName("Light"));
    }
}