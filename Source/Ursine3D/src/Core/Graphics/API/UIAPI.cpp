#include "UrsinePrecompiled.h"
#include "UIAPI.h"

#include "GfxUIManager.h"

namespace ursine
{
    namespace graphics
    {
        struct UIAPI::privData
        {
            GfxUIManager *mgr;
        };


        GfxHND UIAPI::CreateUI()
        {
            return m_privates->mgr->CreateUI();
        }

        UIInstance &UIAPI::GetUI(GfxHND ui)
        {
            return m_privates->mgr->GetUI(ui);
        }

        void UIAPI::DestroyUI(GfxHND ui)
        {
            m_privates->mgr->DestroyUI(ui);
        }

        void UIAPI::SetPrivates(void *priv)
        {
            m_privates->mgr = reinterpret_cast<GfxUIManager*>(priv);
        }

        void UIAPI::Initialize()
        {
            m_privates = new UIAPI::privData();
        }

        void UIAPI::Uninitialize()
        {
            delete m_privates;
        }
    }
}