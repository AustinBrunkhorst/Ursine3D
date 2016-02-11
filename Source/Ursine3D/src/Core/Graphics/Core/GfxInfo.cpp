/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GfxInfo.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "GfxInfo.h"
#include "GfxDefines.h"
#include <d3d11.h>
#include <locale>
#include <codecvt>
#include "DXErrorHandling.h"

namespace ursine
{
    namespace graphics
    {
        using namespace DXCore;

        std::string ws2s(const std::wstring &wstr)
        {
            typedef std::codecvt_utf8<wchar_t> convert_typeX;
            std::wstring_convert<convert_typeX, wchar_t> converterX;

            return converterX.to_bytes(wstr);
        }

        void GfxInfo::Initialize()
        {
            HRESULT result;
            IDXGIFactory *factory;
            IDXGIAdapter *adapter;
            IDXGIOutput *adapterOutput;
            DXGI_ADAPTER_DESC adapterDesc;

            //Create a DirectX graphics interface factory.
            result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
            UAssert(result == S_OK, "Failed to make factory! (Error '%s')", GetDXErrorMessage(result));

            //Use the factory to create an adapter for the primary graphics interface (video card).
            result = factory->EnumAdapters(0, &adapter);
            UAssert(result == S_OK, "Failed to get adapter! (Error '%s')", GetDXErrorMessage(result));

            //Enumerate the primary adapter output (monitor).
            result = adapter->EnumOutputs(0, &adapterOutput);
            UAssert(result == S_OK, "Failed to make adapter enum! (Error '%s')", GetDXErrorMessage(result));

            //Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
            result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_SCALING, &m_modeCount, nullptr);
            UAssert(result == S_OK, "Failed to get sampler state! (Error '%s')", GetDXErrorMessage(result));

            UAssert(m_modeCount > 0, "No modes available");

            //get all of the display modes
            m_displayModeList = new DXGI_MODE_DESC[ m_modeCount ];
            result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &m_modeCount, m_displayModeList);
            UAssert(result == S_OK, "Failed to get display mode list! (Error '%s')", GetDXErrorMessage(result));

            //Get the adapter (video card) description.
            result = adapter->GetDesc(&adapterDesc);
            UAssert(result == S_OK, "Failed to get adapter description (Error '%s')", GetDXErrorMessage(result));

            //get size of video memory
            m_dedicatedVideoMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
            m_sharedVideoMemory = (int)(adapterDesc.SharedSystemMemory / 1024 / 1024);

            LogMessage("Video Card: %s", 2, ws2s(adapterDesc.Description).c_str());
            LogMessage("Dedicated Video Memory: %i", 2, m_dedicatedVideoMemory);
            LogMessage("Shared Video Memory: %i", 2, m_sharedVideoMemory);

            //Release the adapter output.
            adapterOutput->Release();
            adapterOutput = nullptr;

            //Release the adapter.
            adapter->Release();
            adapter = nullptr;

            //Release the factory.
            factory->Release();
            factory = nullptr;

            URSINE_TODO( "Enable multisampling toggling" );
            m_maxSampleCount = 1;
            m_maxSampleQuality = 0;
        }

        void GfxInfo::Uninitialize()
        {
            if (m_displayModeList != nullptr)
                delete[] m_displayModeList;
        }

        DXGI_MODE_DESC &GfxInfo::GetDisplayInfo(unsigned index)
        {
            UAssert(index < m_modeCount, "Index out of gfxinfo");
            return m_displayModeList[ index ];
        }

        unsigned GfxInfo::GetModeCount()
        {
            return m_modeCount;
        }

        unsigned GfxInfo::GetSampleCount()
        {
            return m_maxSampleCount;
        }

        unsigned GfxInfo::GetSampleQuality()
        {
            return m_maxSampleQuality;
        }

        void GfxInfo::GetDimensions(unsigned &w, unsigned &h)
        {
            w = m_windowWidth;
            h = m_windowHeight;
        }

        void GfxInfo::SetDimensions(unsigned w, unsigned h)
        {
            m_windowWidth = w;
            m_windowHeight = h;
        }
    }
}