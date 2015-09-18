#include "UrsinePrecompiled.h"
#include "Camera.h"

void Camera::Initialize( void )
{
  m_position = DirectX::XMVectorSet( 0, 0, -10, 1 );
  LookAtPoint( DirectX::XMFLOAT4( 0, 0, 0, 1 ) );

  m_projMode = PROJECTION_PERSPECTIVE;
  m_fov = 45.f;
}

void Camera::Uninitialize( )
{

}

DirectX::XMMATRIX Camera::GetViewMatrix( )
{
  return DirectX::XMMatrixLookAtLH( m_position, DirectX::XMVectorAdd( m_position, m_look ), m_up );
}

DirectX::XMMATRIX Camera::GetProjMatrix(float width, float height)
{
  if (m_projMode == Camera::ProjectionMode::PROJECTION_PERSPECTIVE)
  {
    return DirectX::XMMatrixPerspectiveFovLH( m_fov * 3.14f / 180.f, width / height, 0.0001f, 1000.f );
  }
  else
  {
    return DirectX::XMMatrixOrthographicLH(width, height, 0.0001f, 1000.f );
  }
}

DirectX::XMFLOAT4 Camera::GetPosition( )
{
  return DirectX::XMFLOAT4( m_position.m128_f32 );
}
void Camera::SetPosition( const DirectX::XMFLOAT4 &pos )
{
  m_position = DirectX::XMLoadFloat4( &pos );
}

DirectX::XMFLOAT4 Camera::GetLook( )
{
  return DirectX::XMFLOAT4( m_look.m128_f32 );
}
void Camera::SetLook( const DirectX::XMFLOAT4 &look )
{
  m_look = DirectX::XMLoadFloat4( &look );
  CalculateVectors( DirectX::XMFLOAT4( 0, 1, 0, 0 ) );
}

DirectX::XMFLOAT4 Camera::GetRight( )
{
  return DirectX::XMFLOAT4( m_right.m128_f32 );
}

DirectX::XMFLOAT4 Camera::GetUp( )
{
  return DirectX::XMFLOAT4( m_up.m128_f32 );
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
void Camera::SetProjMode( Camera::ProjectionMode mode )
{
  m_projMode = mode;
}

void Camera::LookAtPoint( const DirectX::XMFLOAT4 &point )
{
  m_look = DirectX::XMVectorSubtract( DirectX::XMLoadFloat4( &point ), m_position );
  CalculateVectors( DirectX::XMFLOAT4( 0, 1, 0, 0 ) );
}

void Camera::CalculateVectors( DirectX::XMFLOAT4 &up )
{
  m_look = DirectX::XMVector3NormalizeEst( m_look );
  m_right = DirectX::XMVector3NormalizeEst( DirectX::XMVector3Cross( m_look, DirectX::XMLoadFloat4( &up ) ) );
  m_up = DirectX::XMVector3NormalizeEst( DirectX::XMVector3Cross( DirectX::XMVectorNegate( m_look ), m_right ) );
}