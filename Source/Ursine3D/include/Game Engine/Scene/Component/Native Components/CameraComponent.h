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

#include "RenderMask.h"
#include "RenderableComponentBase.h"

namespace ursine
{
    namespace graphics
    {
        class GfxAPI;
    }

    namespace ecs
    {
        enum class CameraRenderMode
        {
            Deferred = graphics::VIEWPORT_RENDER_DEFERRED,
            Forward = graphics::VIEWPORT_RENDER_FORWARD
        } Meta(Enable);

        class Camera 
            : public Component
            , public RenderableComponentBase
        {
            NATIVE_COMPONENT;

        public:
            EditorField(
                ursine::ecs::CameraRenderMode renderMode,
                GetRenderMode,
                SetRenderMode
            );

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

            Meta(BitMaskEditor)
            EditorField(
                ursine::ecs::RenderMask renderMask,
                GetRenderMask,
                SetRenderMask
            );

            EditorField(
                Color clearColor,
                GetClearColor,
                SetClearColor
            );

            Meta(Enable)
            Camera(void);
            ~Camera(void);

            void OnInitialize(void) override;

            void updateRenderer(void);

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

            ursine::ecs::CameraRenderMode GetRenderMode(void);
            void SetRenderMode(ursine::ecs::CameraRenderMode type);

            int GetRenderLayer(void) const;
            void SetRenderLayer(int layer);

            int GetSortLayer(void) const;

            SVec3 GetLook(void);
            void SetLook(const SVec3 &worldPosition);

            SVec3 GetRight(void);
            SVec3 GetUp(void);

            SMat4 GetViewMatrix(void);
            SMat4 GetProjMatrix(void);

            ursine::ecs::RenderMask GetRenderMask(void) const;
            void SetRenderMask(ursine::ecs::RenderMask mask);

            SVec3 ScreenToWorld(const Vec2 &screenPos, float depth) const;
			Vec2 WorldToScreen(const SVec3 &worldPos) const;

			// grabs object position from the gpu
			SVec3 GetMouseWorldPosition(void) const;

            bool IsEditorCamera(void) const;
            void SetEditorCamera(bool editorCamera);

            // Puts the viewport in the bottom right of the scene viewport
            // if selected. Used by the editor
            void SetEditorSelectionMode(bool selected);

            const Color &GetClearColor(void) const;
            void SetClearColor(const Color &color);
        private:
            bool m_active;
            bool m_isEditorCamera;
            bool m_inEditorSelectionMode;

            int m_renderLayer;
			unsigned m_renderMask;

            Vec2 m_viewportPosition;
            Vec2 m_viewportSize;

            graphics::GfxAPI *m_graphics;

        } Meta(Enable, WhiteListMethods, DisplayName( "Camera" ));
    }
}