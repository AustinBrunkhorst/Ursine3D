#pragma once

#include "Component.h"

#include "Camera.h"
#include "GFXAPIDefines.h"

#include "RenderableComponentBase.h"

namespace ursine
{
    namespace ecs
    {
        class Camera 
            : public Component
            , public RenderableComponentBase
        {
            NATIVE_COMPONENT;
        public:
            EditorField(
                Vec2 viewportPosition,
                GetViewportPosition,
                SetViewportPosition
            );

            EditorField(
                Vec2 viewportSize,
                GetViewportSize,
                SetViewportSize
            );

            EditorField(
                float nearPlane,
                GetNearPlane,
                SetNearPlane
            );

            EditorField(
                float farPlane,
                GetFarPlane,
                SetFarPlane
            );

            EditorField(
                float fov,
                GetFOV,
                SetFOV
            );

            Meta(Enable)
            Camera(void);
            ~Camera(void);

            void OnInitialize(void) override;

            void updateRenderer(void) override;

            Vec2 GetViewportPosition(void) const;
            void SetViewportPosition(const Vec2 &position);

            Vec2 GetViewportSize(void) const;
            void SetViewportSize(const Vec2 &size);

            float GetNearPlane(void) const;
            void SetNearPlane(float nearPlane);

            float GetFarPlane(void) const;
            void SetFarPlane(float farPlane);

            float GetFOV(void) const;
            void SetFOV(float fov);

            SVec3 GetLook(void);
            void SetLook(const SVec3 &worldPosition);

            SVec3 GetRight(void);
            SVec3 GetUp(void);

            SMat4 GetViewMatrix(void);
            SMat4 GetProjMatrix(void);

            SVec3 ScreenToWorld(const Vec2 &screenPos, float depth);

        } Meta(Enable, WhiteListMethods, DisplayName( "Camera" ));
    }
}