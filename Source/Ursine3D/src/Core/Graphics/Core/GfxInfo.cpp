#include "UrsinePrecompiled.h"
#include "GfxInfo.h"
#include "GraphicsDefines.h"
#include <locale>
#include <codecvt>

namespace ursine
{
  std::string ws2s( const std::wstring& wstr )
  {
    typedef std::codecvt_utf8<wchar_t> convert_typeX;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.to_bytes( wstr );
  }

  void GfxInfo::Initialize( )
  {
    HRESULT result;
    IDXGIFactory *factory;
    IDXGIAdapter *adapter;
    IDXGIOutput *adapterOutput;
    DXGI_ADAPTER_DESC adapterDesc;

    //Create a DirectX graphics interface factory.
    result = CreateDXGIFactory( __uuidof(IDXGIFactory), (void**)&factory );
    UAssert( result == S_OK, "Failed to make factory! (Error '%i')", result );

    //Use the factory to create an adapter for the primary graphics interface (video card).
    result = factory->EnumAdapters( 0, &adapter );
    UAssert( result == S_OK, "Failed to get adapter! (Error '%i')", result );

    //Enumerate the primary adapter output (monitor).
    result = adapter->EnumOutputs( 0, &adapterOutput );
    UAssert( result == S_OK, "Failed to make adapter enum! (Error '%i')", result );

    //Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
    result = adapterOutput->GetDisplayModeList( DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_SCALING, &ModeCount_, NULL );
    UAssert( result == S_OK, "Failed to get sampler state! (Error '%i')", result );

    UAssert( ModeCount_ > 0, "No modes available" );

    //get all of the display modes
    DisplayModeList_ = new DXGI_MODE_DESC[ ModeCount_ ];
    result = adapterOutput->GetDisplayModeList( DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &ModeCount_, DisplayModeList_ );
    UAssert( result == S_OK, "Failed to get display mode list! (Error '%i')", result );

    //Get the adapter (video card) description.
    result = adapter->GetDesc( &adapterDesc );
    UAssert( result == S_OK, "Failed to get adapter description (Error '%i')", result );

    //get size of video memory
    DedicatedVideoMemory_ = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
    SharedVideoMemory_ = (int)(adapterDesc.SharedSystemMemory / 1024 / 1024);

    LogMessage( "Video Card: %s", 2, ws2s( adapterDesc.Description ).c_str( ) );
    LogMessage( "Dedicated Video Memory: %i", 2, DedicatedVideoMemory_ );
    LogMessage( "Shared Video Memory: %i", 2, SharedVideoMemory_ );

    //Release the adapter output.
    adapterOutput->Release( );
    adapterOutput = NULL;

    //Release the adapter.
    adapter->Release( );
    adapter = NULL;

    //Release the factory.
    factory->Release( );
    factory = NULL;

    // @Matt TEMP, change the sampling later
    SampleCount_ = 1;
    SampleQuality_ = 0;
  }

  void GfxInfo::Uninitialize( )
  {
    if (DisplayModeList_ != NULL) delete[] DisplayModeList_;
  }

  DXGI_MODE_DESC &GfxInfo::GetDisplayInfo( unsigned index )
  {
    UAssert( index < ModeCount_, "Index out of gfxinfo" );
    return DisplayModeList_[ index ];
  }

  unsigned GfxInfo::GetModeCount( )
  {
    return ModeCount_;
  }

  unsigned GfxInfo::GetSampleCount( )
  {
    return SampleCount_;
  }
  unsigned GfxInfo::GetSampleQuality( )
  {
    return SampleQuality_;
  }

  void GfxInfo::GetDimensions( unsigned &w, unsigned &h )
  {
    w = Width_;
    h = Height_;
  }

  void GfxInfo::SetDimensions( unsigned w, unsigned h )
  {
    Width_ = w;
    Height_ = h;
  }
}