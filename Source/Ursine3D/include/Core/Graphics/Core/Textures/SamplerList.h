/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      SamplerList.h
Module:         Graphics
Purpose:        enum for sample types
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
        enum Sampler
        {
            SAMPLER_WRAP_TEX = 0,
            SAMPLER_NO_FILTERING,

            SAMPLER_COUNT
        };
    }
}