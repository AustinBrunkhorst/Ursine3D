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

            // get a directional light
            DirectionalLight &GetDirectionalLight(GfxHND &handle);

            // get a point light
            PointLight &GetPointLight(GfxHND &handle);

            // get a point light
            Primitive &GetPrimitive(GfxHND &handle);

            Billboard2D &GetBillboard2D(GfxHND &handle);

        private:
            privData *m_privates;

        private:
            void SetPrivates(void *priv);
            void Initialize();
            void Uninitialize();
        };
    }
}