/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      Model.h
Module:         Graphics
Purpose:        base model class
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
        struct ModelResource
        {
            ID3D11Buffer *Vertices_[64];
            ID3D11Buffer *Indices_[ 64 ];
            unsigned VertCount_[ 64 ];
            unsigned IndexCount_[ 64 ];
            unsigned MeshCount_;
        };
    }
}