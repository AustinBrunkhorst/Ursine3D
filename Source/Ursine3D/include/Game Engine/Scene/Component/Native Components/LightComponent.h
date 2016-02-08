/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** LightComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "Renderable.h"
#include "RenderableComponentBase.h"

namespace ursine
{
    namespace ecs
    {
        enum class LightType
        {
            Directional = graphics::Light::LIGHT_DIRECTIONAL,
            Point = graphics::Light::LIGHT_POINT,
            Spot = graphics::Light::LIGHT_SPOTLIGHT,
        } Meta(Enable);

        class Light : public Component
        {
            NATIVE_COMPONENT;

			friend class RenderSystem;

        public:
            EditorField(
                LightType Type,
                GetLightType,
                SetLightType
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

            Light(void);
            ~Light(void);

            Meta(Disable)
            void OnInitialize(void) override;

            Meta(Disable)
            ursine::graphics::GfxHND GetHandle(void) const;

            Meta(Disable)
            const ursine::graphics::Light *GetLight(void);

            LightType GetLightType(void);
            void SetLightType(LightType type);

            const SVec3 &GetDirection(void);
            void SetDirection(const SVec3 &dir);

            const SVec3 &GetPosition(void);
            void SetPosition(const SVec3 &position);

            const ursine::Color &GetColor(void);
            void SetColor(const ursine::Color &color);

            float GetRadius(void);
            void SetRadius(const float radius);

            float GetIntensity(void);
            void SetIntensity(const float intensity);

            const Vec2 &GetSpotlightAngles(void);
            void SetSpotlightAngles(const Vec2 &angles);

            //private methods
        private:
			void updateRenderer(void);

            graphics::Light *m_light;

			RenderableComponentBase *m_base;

            friend class RenderSystem;
        } Meta(Enable, DisplayName( "Light" ));
    }
}