/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RenderableAPI.h
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
File Name:      RenderableAPI.h
Module:         Graphics
Purpose:        API for managing renderables
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include "GFXAPIDefines.h"
#include "Renderable.h"

namespace ursine
{
    namespace graphics
    {
        class RenderableAPI
        {
            friend class GfxAPI;
        private:
            struct privData;

        public:
            // create a graphics object
            GfxHND AddRenderable(const RENDERABLE_TYPE type);

            // destroy graphics object
            void DestroyRenderable(GfxHND &handle);

            // get a model
            Model3D &GetModel3D(GfxHND &handle);

            //get billboarded quad
            Billboard2D &GetBillboard2D(GfxHND &handle);

            //get a universal light
            Light &GetLight(GfxHND &handle);

            ParticleSystem &GetParticleSystem(GfxHND &handle);

        private:
            privData *m_privates;

        private:
            void SetPrivates(void *priv);
            void Initialize();
            void Uninitialize();
        };
    }
}