#include "UrsinePrecompiled.h"

#include "CameraManager.h"

namespace ursine
{
  void CameraManager::Initialize( )
  {
    m_cameraArray.clear( );
    m_cameraArray.push_back( Camera( ) );
    m_cameraArray[ 0 ].Initialize( );

    m_current = 0;
  }

  void CameraManager::Uninitialize( )
  {
    m_cameraArray.clear( );
  }

  GFXHND CameraManager::CreateCamera( )
  {
    GFXHND data;
    _RESOURCEHND *newRender = reinterpret_cast<_RESOURCEHND*>(&data);
    newRender->ID_ = ID_CAMERA;
    newRender->Index_ = m_cameraArray.size( );
    m_cameraArray.push_back( Camera( ) );
    m_cameraArray[ newRender->Index_ ].Initialize( );

    return data;
  }

  Camera &CameraManager::GetCamera( const GFXHND &handle )
  {
    const _RESOURCEHND *newRender = reinterpret_cast<const _RESOURCEHND*>(&handle);

    UAssert( newRender->ID_ == ID_CAMERA, "Attempted to use invalid handle to get camera!" );

    return m_cameraArray[ newRender->Index_ ];
  }

  Camera &CameraManager::GetCurrent( )
  {
    return m_cameraArray[ m_current ];
  }

  void CameraManager::SetCurrent( const GFXHND &handle )
  {
    const _RESOURCEHND *newRender = reinterpret_cast<const _RESOURCEHND*>(&handle);

    UAssert( newRender->ID_ == ID_CAMERA, "Attempted to use invalid handle to get camera!" );

    m_current = newRender->Index_;
  }
}