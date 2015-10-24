/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      CameraManager.h
Module:         Graphics
Purpose:        manager of cameras and handles
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include <vector>
#include <list>

#include "GfxDefines.h"
#include "Camera.h"

namespace ursine
{
    namespace graphics
    {
        class CameraManager
        {
        public:
            void Initialize(void);
            void Uninitialize(void);

            GfxHND CreateCamera(void);
            Camera &GetCamera(const GfxHND &handle);

            void DestroyCamera(GfxHND &handle);

        private:
            std::vector<Camera> m_cameraArray;
            std::list<unsigned> m_freeCameraList;
            unsigned m_current;
        };
    }
}
