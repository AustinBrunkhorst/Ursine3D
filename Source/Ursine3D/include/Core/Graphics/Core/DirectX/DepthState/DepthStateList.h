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

enum DEPTH_STATES
{
  DEPTH_STATE_DEPTH_CHECK = 0,
  DEPTH_STATE_NO_DEPTH_CHECK,
  DEPTH_STATE_COUNT
};