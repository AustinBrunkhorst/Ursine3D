#include "UrsinePrecompiled.h"
#include "Camera.h"
#include "DirectXMath.h"

using namespace ursine;

void Camera::Initialize( void )
{
    m_position.Set( 0, 0, -10 );
    LookAtPoint( SVec3( 0, 0, 0 ) );
    
    m_projMode = PROJECTION_PERSPECTIVE;
    m_fov = 45.f;

    m_nearPlane = 0.1;
    m_farPlane = 100.f;
    m_size = 10.f;
}

void Camera::Uninitialize( )
{

}

SMat4 Camera::GetViewMatrix( )
{
	auto eye = SVec4(m_position, 1.0f).ToD3D();
	auto target = SVec4(m_position + m_look, 1).ToD3D();
	auto up = SVec4(m_up, 0).ToD3D();

	auto eyePos = XMLoadFloat4(&eye);
	auto targetPos = XMLoadFloat4(&target);
	auto upDir = XMLoadFloat4(&up);

	return SMat4( DirectX::XMMatrixLookAtLH( eyePos, targetPos, upDir ) );
}

SMat4 Camera::GetProjMatrix(float width, float height)
{
	SMat4 mat;

    if (m_projMode == PROJECTION_PERSPECTIVE)
    {
        return SMat4(DirectX::XMMatrixPerspectiveFovLH( m_fov * 3.14f / 180.f, width / height, m_nearPlane, m_farPlane ));
    }
    else
    {
        return SMat4(DirectX::XMMatrixOrthographicLH( m_size * width/height, m_size, m_nearPlane, m_farPlane ));
    }
}

SVec3 Camera::GetPosition( )
{
    return m_position;
}
void Camera::SetPosition( const SVec3 &pos )
{
    m_position = pos;
}

SVec3 Camera::GetLook( )
{
    return m_look;
}
void Camera::SetLook( const SVec3 &look )
{
    m_look = look;
    CalculateVectors( SVec3::UnitY( ) );
}

SVec3 Camera::GetRight( )
{
    return m_right;
}

SVec3 Camera::GetUp( )
{
    return m_up;
}

void Camera::SetPlanes ( float nearPlane, float farPlane )
{
  UAssert( nearPlane < farPlane, "Near and far planes cannot be the same!" );
  m_nearPlane = nearPlane;
  m_farPlane = farPlane;
}

void Camera::GetPlanes ( float& nearPlane, float& farPlane )
{
  nearPlane = m_nearPlane;
  farPlane = m_farPlane;
}

float Camera::GetFOV( )
{
    return m_fov;
}
void Camera::SetFOV( float fov )
{
    UAssert( fov > 0, "FOV can't be less than 0!" );
    m_fov = fov;
}

Camera::ProjectionMode Camera::GetProjMode( )
{
    return m_projMode;
}
void Camera::SetProjMode( ProjectionMode mode )
{
    m_projMode = mode;
}

void Camera::SetSize ( float size )
{
  m_size = size;
}

void Camera::LookAtPoint( const SVec3 &point )
{
  m_look = point - m_position;
  CalculateVectors( SVec3::UnitY( ) );
}

void Camera::CalculateVectors( const SVec3 &up )
{
  m_look.Normalize();
  
  m_right = SVec3::Cross(m_look, up);
  m_right.Normalize();
  
  m_up = SVec3::Cross(-m_look, m_right);
  m_up.Normalize();
}