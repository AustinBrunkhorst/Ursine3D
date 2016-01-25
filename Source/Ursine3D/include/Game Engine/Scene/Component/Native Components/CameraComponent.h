/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CameraComponent.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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

            EditorField(
                bool active,
                GetActive,
                SetActive
            );

            EditorField(
                int renderLayer,
                GetRenderLayer,
                SetRenderLayer
            );

            EditorField(
                int renderMask,
                GetRenderMask,
                SetRenderMask
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

            bool GetActive(void) const;
            void SetActive(bool active);

            int GetRenderLayer(void) const;
            void SetRenderLayer(int layer);

            SVec3 GetLook(void);
            void SetLook(const SVec3 &worldPosition);

            SVec3 GetRight(void);
            SVec3 GetUp(void);

            SMat4 GetViewMatrix(void);
            SMat4 GetProjMatrix(void);

            int GetRenderMask(void) const;
            void SetRenderMask(const int mask);

            SVec3 ScreenToWorld(const Vec2 &screenPos, float depth) const;
			Vec2 WorldToScreen(const SVec3 &worldPos) const;

			// grabs object position from the gpu
			SVec3 GetMouseWorldPosition(void) const;

        private:

            bool m_active;

            int m_renderLayer;

        } Meta(Enable, WhiteListMethods, DisplayName( "Camera" ));
    }
}