/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      DepthStateList.h
Module:         Graphics
Purpose:        enum of possible depth states
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/
#pragma once

namespace ursine
{
    namespace graphics
    {
        enum DEPTH_STENCIL_LIST
        {
            DEPTH_STENCIL_MAIN = 0,
            DEPTH_STENCIL_SHADOWMAP,

            DEPTH_STENCIL_COUNT
        };
    }
}
