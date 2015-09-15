/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      ShaderList.h
Module:         Graphics
Purpose:        list of the current shaders in the engine
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

enum SHADER_TYPES
{
  SHADER_BASIC = 0,
  SHADER_QUAD,
  SHADER_DIFFUSE,
  SHADER_NORMAL,
  SHADER_DEFFERED_TEXTURE,
  SHADER_DEFERRED_DEPTH,
  SHADER_DEFERRED_DEPTH_NORM,
  SHADER_UI,

  //lights need to go after this, post processing stuffz...
  SHADER_POINT_LIGHT,         //this order is important
  SHADER_DIRECTIONAL_LIGHT,

  //and past this is debug!
  SHADER_PRIMITIVE,

  //shaders not part of the regular pipeline
  SHADER_POINT,
  SHADER_SHADOW,

  SHADER_COUNT
};

