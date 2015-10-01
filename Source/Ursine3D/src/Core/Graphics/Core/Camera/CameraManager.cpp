#include "UrsinePrecompiled.h"

#include "CameraManager.h"
#include "GraphicsDefines.h"

namespace ursine
{
    void CameraManager::Initialize( void )
    {
        m_cameraArray.resize( MAX_CAMERAS );

        for (unsigned x = 0; x < MAX_CAMERAS; ++x)
        {
            m_freeCameraList.push_back( x );
        }
    }

    void CameraManager::Uninitialize( void )
    {
        
    }

    GFXHND CameraManager::CreateCamera()
    {
        UAssert( m_freeCameraList.size( ) > 0, "Out of cameras!" );
        GFXHND data;
        _RESOURCEHND *newRender = reinterpret_cast<_RESOURCEHND*>( &data );
        newRender->ID_ = ID_CAMERA;
        newRender->Index_ = m_freeCameraList.front( );
        m_freeCameraList.pop_front( );
        m_cameraArray[ newRender->Index_ ].Initialize( );

        return data;
    }

    GFXCamera &CameraManager::GetCamera(const GFXHND &handle)
    {
        const _RESOURCEHND *newRender = reinterpret_cast<const _RESOURCEHND*>( &handle );

        UAssert( newRender->ID_ == ID_CAMERA, "Attempted to use invalid handle to get camera!" );

        return m_cameraArray[ newRender->Index_ ];
    }

    void CameraManager::DestroyCamera(GFXHND& handle)
    {
        _RESOURCEHND *newRender = reinterpret_cast<_RESOURCEHND*>(&handle);

        UAssert( newRender->ID_ == ID_CAMERA, "Attempted to destroy invalid camera handle!" );
        m_cameraArray[ newRender->Index_ ].Uninitialize( );
        m_freeCameraList.push_front( newRender->ID_ );
        handle = 0;
    }
}
