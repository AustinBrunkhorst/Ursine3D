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
#include "SelectedComponent.h"

#include "SystemManager.h"
#include "RenderSystem.h"

#include "GfxAPI.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( Camera );

        namespace
        {
            // size in percentage of the selected camera
            const Vec2 kEditorSelectedSize = Vec2 { 0.35f, 0.35f };

            // padding in percentage to offset the selected camera from the bottom right corner
            const Vec2 kEditorSelectedPadding = Vec2 { 0.01f, 0.01f };

            // location after the configuration above
            const Vec2 kEditorSelectedLocation = 
                Vec2 { 1.0f, 1.0f } - kEditorSelectedSize - kEditorSelectedPadding;
        }

        Camera::Camera(void)
            : BaseComponent( )
            , RenderableComponentBase( std::bind( &Camera::updateRenderer, this ) )
            , m_active( true )
            , m_isEditorCamera( false )
            , m_inEditorSelectionMode( false )
            , m_renderLayer( 0 )
            , m_renderMask( 0 )
            , m_graphics( GetCoreSystem( graphics::GfxAPI ) )
        {
            m_handle = m_graphics->CameraMgr.AddCamera( );
            m_graphics->CameraMgr.GetCamera( m_handle ).SetMask( 0 );

            SetNearPlane( 0.1f );
            SetFarPlane( 1000.0f );
        }

        Camera::~Camera(void)
        {
            RenderableComponentBase::OnRemove( GetOwner( ) );

            m_graphics->CameraMgr.DestroyCamera( m_handle );
        }

        void Camera::OnInitialize(void)
        {
            RenderableComponentBase::OnInitialize( GetOwner( ) );

            // make sure we are selected
            if (GetOwner( )->HasComponent<Selected>( ))
                SetEditorSelectionMode( true );
        }

        void Camera::updateRenderer(void)
        {
            auto trans = GetOwner( )->GetTransform( );
            auto &cam = m_graphics->CameraMgr.GetCamera( m_handle );

            cam.SetPosition( trans->GetWorldPosition( ) );
            cam.SetLook( trans->GetForward( ) );
        }

        Vec2 Camera::GetViewportPosition(void) const
        {
            return m_viewportPosition;
        }

        void Camera::SetViewportPosition(const Vec2 &position)
        {
            m_viewportPosition = position;

            if (!m_inEditorSelectionMode)
                m_graphics->CameraMgr.GetCamera( m_handle ).SetViewportPosition( position.X( ), position.Y( ) );
        }

        Vec2 Camera::GetViewportSize(void) const
        {
            return m_viewportSize;
        }

        void Camera::SetViewportSize(const Vec2 &size)
        {
            m_viewportSize = size;

            if (!m_inEditorSelectionMode)
                m_graphics->CameraMgr.GetCamera( m_handle ).SetDimensions( size.X( ), size.Y( ) );
        }

        float Camera::GetNearPlane(void) const
        {
            float nearP, farP;

            m_graphics->CameraMgr.GetCamera( m_handle ).GetPlanes( nearP, farP );

            return nearP;
        }

        void Camera::SetNearPlane(float nearPlane)
        {
            m_graphics->CameraMgr.GetCamera( m_handle ).SetPlanes( nearPlane, GetFarPlane( ) );
        }

        float Camera::GetFarPlane(void) const
        {
            float nearP, farP;

            m_graphics->CameraMgr.GetCamera( m_handle ).GetPlanes( nearP, farP );

            return farP;
        }

        void Camera::SetFarPlane(float farPlane)
        {
            m_graphics->CameraMgr.GetCamera( m_handle ).SetPlanes( GetNearPlane( ), farPlane );
        }

        float Camera::GetFOV(void) const
        {
            return m_graphics->CameraMgr.GetCamera( m_handle ).GetFOV( );
        }

        void Camera::SetFOV(float fov)
        {
            m_graphics->CameraMgr.GetCamera( m_handle ).SetFOV( fov );
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
            if (!m_inEditorSelectionMode)
                GetOwner( )->GetWorld( )->GetEntitySystem( RenderSystem )->SortCameraArray( );
        }

        int Camera::GetSortLayer(void) const
        {
            return m_inEditorSelectionMode ? std::numeric_limits<int>::max( ) : m_renderLayer;
        }

        SVec3 Camera::GetLook(void)
        {
            return GetOwner( )->GetTransform( )->GetForward( );
        }

        void Camera::SetLook(const SVec3 &look)
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
            return m_graphics->CameraMgr.GetCamera( m_handle ).GetViewMatrix( );
        }

        SMat4 Camera::GetProjMatrix(void)
        {
            return m_graphics->CameraMgr.GetCamera( m_handle ).GetProjMatrix( );
        }

        unsigned Camera::GetRenderMask(void) const
        {
            return m_renderMask;
        }

        void Camera::SetRenderMask(const unsigned mask)
        {
            m_renderMask = mask;

            if (!m_inEditorSelectionMode)
                m_graphics->CameraMgr.GetCamera( m_handle ).SetMask( mask );
        }

        SVec3 Camera::ScreenToWorld(const Vec2 &screenPos, float depth) const
        {
            return m_graphics->CameraMgr.GetCamera( m_handle ).ScreenToWorld( screenPos, depth );
        }

        Vec2 Camera::WorldToScreen(const SVec3 &worldPos) const
        {
            auto &cam = m_graphics->CameraMgr.GetCamera( m_handle );

            auto viewMatrix = SMat4::Transpose( cam.GetViewMatrix( ) );
            auto projMatrix = SMat4::Transpose( cam.GetProjMatrix( ) );

            SVec4 point = SVec4( worldPos, 1.0f );

            point = viewMatrix * point;
            point = projMatrix * point;

            return { point.X( ), point.Y( ) };
        }

        SVec3 Camera::GetMouseWorldPosition(void) const
        {
            return m_graphics->GetMousedOverWorldPosition( m_handle );
        }

        bool Camera::IsEditorCamera(void) const
        {
            return m_isEditorCamera;
        }

        void Camera::SetEditorCamera(bool editorCamera)
        {
            m_isEditorCamera = editorCamera;
        }

        void Camera::SetEditorSelectionMode(bool selected)
        {
            // don't modify the editor camera
            if (m_isEditorCamera)
                return;

            m_inEditorSelectionMode = selected;

            auto &camera = m_graphics->CameraMgr.GetCamera( m_handle );

            if (selected)
            {
                auto excludeEditorToolMask = m_renderMask;

                utils::FlagSet( excludeEditorToolMask, MEditorTool );

                camera.SetViewportPosition( kEditorSelectedLocation.X( ), kEditorSelectedLocation.Y( ) );
                camera.SetDimensions( kEditorSelectedSize.X( ), kEditorSelectedSize.Y( ) );
                camera.SetMask( excludeEditorToolMask );
            }
            else
            {
                camera.SetViewportPosition( m_viewportPosition.X( ), m_viewportPosition.Y( ) );
                camera.SetDimensions( m_viewportSize.X( ), m_viewportSize.Y( ) );
                camera.SetMask( m_renderMask );
            }

            auto *renderSystem = GetOwner( )->GetWorld( )->GetEntitySystem( RenderSystem );

            if (renderSystem)
                renderSystem->SortCameraArray( );
        }
    }
}
