#include "UrsinePrecompiled.h"
#include "CameraAPI.h"
#include "CameraManager.h"

namespace ursine
{
    struct CameraAPI::privData
    {
        CameraManager *mgr;
    };

    GFXHND CameraAPI::AddCamera()
    {
        return m_privates->mgr->CreateCamera( );
    }

    GFXCamera &CameraAPI::GetCamera(const GFXHND &handle)
    {
        return m_privates->mgr->GetCamera( handle );
    }

    void CameraAPI::DestroyCamera(GFXHND &handle)
    {
        m_privates->mgr->DestroyCamera( handle );
    }

    void CameraAPI::SetPrivates(void *priv)
    {
        m_privates->mgr = reinterpret_cast<CameraManager*>( priv );
    }

    void CameraAPI::Initialize()
    {
        m_privates = new CameraAPI::privData( );
    }

    void CameraAPI::Uninitialize()
    {
        delete m_privates;
    }
}
