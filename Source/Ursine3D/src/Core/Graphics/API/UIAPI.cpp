#include "UrsinePrecompiled.h"
#include "UIAPI.h"

#include "GfxUIManager.h"

namespace ursine
{
    struct UIAPI::privData
    {
        GfxUIManager *mgr;
    };


    GFXHND UIAPI::CreateUI()
    {
        return m_privates->mgr->CreateUI( );
    }

    UIInstance &UIAPI::GetUI(GFXHND ui)
    {
        return m_privates->mgr->GetUI( ui );
    }

    void UIAPI::DestroyUI(GFXHND ui)
    {
        m_privates->mgr->DestroyUI( ui );
    }

    void UIAPI::SetPrivates(void *priv)
    {
        m_privates->mgr = reinterpret_cast<GfxUIManager*>( priv );
    }

    void UIAPI::Initialize()
    {
        m_privates = new UIAPI::privData( );
    }

    void UIAPI::Uninitialize()
    {
        delete m_privates;
    }
}
