/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      GfxInfo.h
Module:         Graphics
Purpose:        class for managing information about the hardware
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/
#pragma once

#include "D3D11Forward.h"

namespace ursine
{
  class GfxInfo
  {
  public:
    void Initialize( );
    void Uninitialize( );

    DXGI_MODE_DESC &GetDisplayInfo( unsigned index );
    unsigned GetModeCount( );

    unsigned GetSampleCount( );
    unsigned GetSampleQuality( );

    void GetDimensions( unsigned &w, unsigned &h );
    void SetDimensions( unsigned w, unsigned h );
  private:
    unsigned ModeCount_;
    DXGI_MODE_DESC *DisplayModeList_;
    unsigned int DedicatedVideoMemory_;
    unsigned int SharedVideoMemory_;
    bool VSync_;
    unsigned Width_;
    unsigned Height_;

    unsigned SampleCount_;
    unsigned SampleQuality_;
  };
}