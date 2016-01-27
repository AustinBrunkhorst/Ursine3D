/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CameraComponent.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "CameraComponent.h"
#include "SystemManager.h"
#include "RenderSystem.h"

#include "GfxAPI.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( Camera );

        Camera::Camera(void)
            : BaseComponent( )
			, RenderableComponentBase([=] { updateRenderer( ); })
            , m_active( true )
            , m_renderLayer( 0 )
        {
            m_handle = GetCoreSystem( graphics::GfxAPI )->CameraMgr.AddCamera( );

            SetNearPlane( 0.1f );
            SetFarPlane( 1000.0f );
        }

        Camera::~Camera(void)
        {
            RenderableComponentBase::OnRemove( GetOwner( ) );
            GetCoreSystem( graphics::GfxAPI )->CameraMgr.DestroyCamera( m_handle );
        }

        void Camera::OnInitialize(void)
        {
            Component::OnInitialize( );

            RenderableComponentBase::OnInitialize( GetOwner( ) );
        }

        void Camera::updateRenderer(void)
        {
            auto trans = GetOwner( )->GetTransform( );
            auto &cam = GetCoreSystem( graphics::GfxAPI )->CameraMgr.GetCamera( m_handle );

            cam.SetPosition( trans->GetWorldPosition( ) );
            cam.SetLook( trans->GetForward( ) );
        }

        Vec2 Camera::GetViewportPosition(void) const
        {
            Vec2 pos;

            GetCoreSystem( graphics::GfxAPI )
                ->CameraMgr.GetCamera( m_handle ).GetViewportPosition( pos.X( ), pos.Y( ) );

            return pos;
        }

        void Camera::SetViewportPosition(const Vec2& position)
        {
            GetCoreSystem( graphics::GfxAPI )
                ->CameraMgr.GetCamera( m_handle ).SetViewportPosition( position.X( ), position.Y( ) );
        }

        Vec2 Camera::GetViewportSize(void) const
        {
            Vec2 size;

            GetCoreSystem( graphics::GfxAPI )
                ->CameraMgr.GetCamera( m_handle ).GetDimensions( size.X( ), size.Y( ) );

            return size;
        }

        void Camera::SetViewportSize(const Vec2& size)
        {
            GetCoreSystem( graphics::GfxAPI )->CameraMgr.GetCamera( m_handle ).SetDimensions( size.X( ), size.Y( ) );
        }

        float Camera::GetNearPlane(void) const
        {
            float nearP, farP;

            GetCoreSystem( graphics::GfxAPI )->CameraMgr.GetCamera( m_handle ).GetPlanes( nearP, farP );

            return nearP;
        }

        void Camera::SetNearPlane(float nearPlane)
        {
            GetCoreSystem( graphics::GfxAPI )->CameraMgr.GetCamera( m_handle ).SetPlanes( nearPlane, GetFarPlane( ) );
        }

        float Camera::GetFarPlane(void) const
        {
            float nearP, farP;

            GetCoreSystem( graphics::GfxAPI )->CameraMgr.GetCamera( m_handle ).GetPlanes( nearP, farP );

            return farP;
        }

        void Camera::SetFarPlane(float farPlane)
        {
            GetCoreSystem( graphics::GfxAPI )->CameraMgr.GetCamera( m_handle ).SetPlanes( GetNearPlane( ), farPlane );
        }

        float Camera::GetFOV(void) const
        {
            return GetCoreSystem( graphics::GfxAPI )->CameraMgr.GetCamera( m_handle ).GetFOV( );
        }

        void Camera::SetFOV(float fov)
        {
            GetCoreSystem( graphics::GfxAPI )->CameraMgr.GetCamera( m_handle ).SetFOV( fov );
        }

        bool Camera::GetActive(void) const
        {
            return m_active;
        }

        void Camera::SetActive(bool active)
        {
            m_active = active;
        }

        int Camera::GetRenderLayer(void) const
        {
            return m_renderLayer;
        }

        void Camera::SetRenderLayer(int layer)
        {
            m_renderLayer = layer;
            
            // notify rendersystem that I've changed
            GetOwner( )->GetWorld( )->GetEntitySystem( RenderSystem )->SortCameraArray( );
        }

        SVec3 Camera::GetLook(void)
        {
            return GetOwner( )->GetTransform( )->GetForward( );
        }

        void Camera::SetLook(const SVec3& look)
        {
            GetOwner( )->GetTransform( )->LookAt( look );
        }

        SVec3 Camera::GetRight(void)
        {
            return GetOwner( )->GetTransform( )->GetRight( );
        }

        SVec3 Camera::GetUp(void)
        {
            return GetOwner( )->GetTransform( )->GetUp( );
        }

        SMat4 Camera::GetViewMatrix(void)
        {
            return GetCoreSystem( graphics::GfxAPI )->CameraMgr.GetCamera( m_handle ).GetViewMatrix( );
        }

        SMat4 Camera::GetProjMatrix(void)
        {
            return GetCoreSystem( graphics::GfxAPI )->CameraMgr.GetCamera( m_handle ).GetProjMatrix( );
        }

        int Camera::GetRenderMask() const
        {
            return GetCoreSystem( graphics::GfxAPI )->CameraMgr.GetCamera( m_handle ).GetMask( );
        }

        void Camera::SetRenderMask(const int mask)
        {
            GetCoreSystem( graphics::GfxAPI )->CameraMgr.GetCamera( m_handle ).SetMask( mask );
        }

        SVec3 Camera::ScreenToWorld(const Vec2& screenPos, float depth) const
        {
            return GetCoreSystem(graphics::GfxAPI)->CameraMgr.GetCamera(m_handle).ScreenToWorld( screenPos, depth );
        }

		Vec2 Camera::WorldToScreen(const SVec3 &worldPos) const
        {
			auto &cam = GetCoreSystem(graphics::GfxAPI)->CameraMgr.GetCamera(m_handle);
	        auto viewMatrix = SMat4::Transpose( cam.GetViewMatrix( ) );
			auto projMatrix = SMat4::Transpose( cam.GetProjMatrix( ) );

			SVec4 point = SVec4( worldPos, 1.0f );

			point = viewMatrix * point;
			point = projMatrix * point;

			return{ point.X(), point.Y() };			
        }

	    SVec3 Camera::GetMouseWorldPosition(void) const
	    {
			return GetCoreSystem(graphics::GfxAPI)->GetMousedOverWorldPosition(m_handle);
	    }
    }
}