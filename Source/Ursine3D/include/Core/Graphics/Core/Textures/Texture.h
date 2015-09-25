/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      Texture.h
Module:         Graphics
Purpose:        Texture class
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include <d3d11.h>

namespace ursine
{
  struct Texture
  {
    unsigned Width_;
    unsigned Height_;
    ID3D11ShaderResourceView *Texture_;
  };
}