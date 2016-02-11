/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GfxInfo.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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
    namespace graphics
    {
        class GfxInfo
        {
        public:
            void Initialize();
            void Uninitialize();

            DXGI_MODE_DESC &GetDisplayInfo(unsigned index);
            unsigned GetModeCount();

            unsigned GetSampleCount();
            unsigned GetSampleQuality();

            void GetDimensions(unsigned &w, unsigned &h);
            void SetDimensions(unsigned w, unsigned h);
        private:
            unsigned m_modeCount;
            DXGI_MODE_DESC *m_displayModeList;
            unsigned int m_dedicatedVideoMemory;
            unsigned int m_sharedVideoMemory;
            bool m_useVSync;
            unsigned m_windowWidth;
            unsigned m_windowHeight;

            unsigned m_maxSampleCount;
            unsigned m_maxSampleQuality;
        };
    }
}
