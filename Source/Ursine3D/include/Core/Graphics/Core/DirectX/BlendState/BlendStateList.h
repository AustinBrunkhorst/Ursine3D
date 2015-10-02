/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      BlendStateList.h
Module:         Graphics
Purpose:        Enum of blend states
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/
#pragma once

enum BLEND_STATES
{
    BLEND_STATE_DEFAULT = 0,
    BLEND_STATE_ADDITIVE,
    BLEND_STATE_NONE,

    BLEND_STATE_COUNT
};
