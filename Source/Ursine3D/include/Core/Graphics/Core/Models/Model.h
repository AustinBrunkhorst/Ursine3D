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
    struct ModelResource
    {
        ID3D11Buffer *Vertices_;
        ID3D11Buffer *Indices_;
        unsigned VertCount_;
        unsigned IndexCount_;
    };
}
