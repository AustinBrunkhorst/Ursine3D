/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Camera.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/


#pragma once

#include "SMat4.h"
#include "ViewportRenderModes.h"
#include "EntityConfig.h"
#include "Color.h"

namespace ursine
{
    namespace graphics
    {
        class Camera
        {
        public:
            enum ProjectionMode
            {
                PROJECTION_PERSPECTIVE = 0,
                PROJECTION_ORTHOGRAPHIC,

                PROJECTION_COUNT
            };

        public:
            //initializing
            void Initialize(void);
            void Uninitialize();

            //getting matrices
            ursine::SMat4 GetViewMatrix() const;
            ursine::SMat4 GetProjMatrix() const;

            //setting position
            const ursine::SVec3 &GetPosition() const;
            void SetPosition(const ursine::SVec3 &pos);

            //get look direction
            const ursine::SVec3 &GetLook() const;
            void SetLook(const ursine::SVec3 &dir);

            //getting right and up
            const ursine::SVec3 &GetRight(void) const;
            const ursine::SVec3 &GetUp(void) const;

            //near/far planes
            void SetPlanes(const float nearPlane, const float farPlane);
            void GetPlanes(float &nearPlane, float &farPlane) const;

            //field of view
            float GetFOV(void) const;
            void SetFOV(float fov);

            //projection mode
            ProjectionMode GetProjMode(void) const;
            void SetProjMode(const ProjectionMode mode);

            //orthographic size
            float GetSize(void);
            void SetSize(const float size);

            //look at point in world space
            void LookAtPoint(const ursine::SVec3 &point);

            //set dimensions w/ respect to main viewport from 0 to 1
            void SetDimensions(const float width, const float height);
            void GetDimensions(float &width, float &height) const;

            //set the position w/ respect to main viewport
            //0, 0 is top left corner
            void SetViewportPosition(const float x, const float y);
            void GetViewportPosition(float &x, float &y) const;

            //set render mode (orthographics VS perspective)
            void SetRenderMode(const ViewportRenderMode renderMode);

            //get the render mode
            ViewportRenderMode GetRenderMode(void) const;

            //convert screen point to world point
            SVec3 ScreenToWorld(const Vec2 &screenPos, const float depth);

            //DO NOT CALL set screen dimensions
            void SetScreenDimensions(const float width, const float height);

            //DO NOT CALL set screen pixel positions
            void SetScreenPosition(const float x, const float y);

            // check to see if a given mask will pass this camera's mask
            bool CheckMask(const unsigned long long renderMask);

            // set the mask this camera will use for culling operations
            unsigned GetMask(void) const;
            void SetMask(const unsigned long long renderMask);

            ecs::EntityID GetEntityID(void) const;
            void SetEntityID(const ecs::EntityID id);

            const Color &GetClearColor(void) const;
            void SetClearColor(const Color &color);

        private:
            // calcualte all vectors based upon up
            void CalculateVectors(const ursine::SVec3 &up);

            // render mask for culling objects on a layer-level
            unsigned long long m_cameraMask;

            // entity ID
            ecs::EntityID m_entityID;

            // field of view, near, var
            float m_fov;
            float m_nearPlane;
            float m_farPlane;

            // orhtographic size
            float m_size;

            // ortho or perspective projection
            ProjectionMode m_projMode;

            // general camera data
            ursine::SVec3 m_position;
            ursine::SVec3 m_right, m_up, m_look;
            ursine::SMat4 m_view;

            // forward or deferred?
            ViewportRenderMode m_renderMode;

            // these are the dimensions of this camera's viewport
            float m_width;
            float m_height;
            float m_xPos;
            float m_yPos;

            // these are the actual pixel-position/sizes of the screen.
            float m_screenX;
            float m_screenY;
            float m_screenWidth;
            float m_screenHeight;

            Color m_clearColor;
        };
    }
}