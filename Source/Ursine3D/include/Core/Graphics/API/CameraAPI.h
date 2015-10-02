/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      CameraAPI.h
Module:         Graphics
Purpose:        Interface for camera class
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include "GFXAPIDefines.h"
#include "Camera.h"

namespace ursine
{
    namespace graphics
    {
        class CameraAPI
        {
            friend class GfxAPI;
        private:
            struct privData;

        public:
            // Create an camera
            GfxHND AddCamera();
            // get a camera
            Camera &GetCamera(const GfxHND &handle);
            // set active game camera
            void DestroyCamera(GfxHND &handle);

        private:
            privData *m_privates;

        private:
            void SetPrivates(void *priv);
            void Initialize();
            void Uninitialize();
        };
    }
}