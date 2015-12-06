/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RenderableManager.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
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
File Name:      RenderableManager.h
Module:         Graphics
Purpose:        Class manager for holding all renderables
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#include <vector>
#include <list> 

#include "GfxDefines.h"
#include "GfxHandle.h"
#include "RenderableTypes.h"
#include "Renderable.h"

namespace ursine
{
    namespace graphics
    {
        class Model3D;

        class RenderableManager
        {
            friend class GfxManager;
        public:
            void Initialize();
            void Uninitialize();

            GfxHND AddRenderable(const RENDERABLE_TYPE type);
            void DestroyRenderable(GfxHND &handle);

            template<typename T>
            T &GetRenderable(GfxHND handle)
            {
                return T();
            }

            template<>
            Model3D &GetRenderable<Model3D>(GfxHND handle)
            {
                _RENDERABLEHND *render = HND_RENDER(handle);

                UAssert(render->ID_ == ID_RENDERABLE, "Attempted to get renderable from non-valid handle!");
                UAssert(render->Type_ == RENDERABLE_MODEL3D, "Attempted to use invalid handle to get a model!");

                return m_renderableModel3D[ render->Index_ ];
            }

            template<>
            Primitive &GetRenderable<Primitive>(GfxHND handle)
            {
                _RENDERABLEHND *render = HND_RENDER(handle);

                UAssert(render->ID_ == ID_RENDERABLE, "Attempted to get renderable from non-valid handle!");
                UAssert(render->Type_ == RENDERABLE_PRIMITIVE, "Attempted to use invalid handle to get a primitive!");

                return m_renderablePrimitives[ render->Index_ ];
            }

            template<>
            Billboard2D &GetRenderable<Billboard2D>(GfxHND handle)
            {
                _RENDERABLEHND *render = HND_RENDER(handle);

                UAssert(render->ID_ == ID_RENDERABLE, "Attempted to get renderable from non-valid handle!");
                UAssert(render->Type_ == RENDERABLE_BILLBOARD2D, "Attempted to use invalid handle to get a billboard2D!");

                return m_renderableBillboards[ render->Index_ ];
            }

            template<>
            Light &GetRenderable<Light>(GfxHND handle)
            {
                _RENDERABLEHND *render = HND_RENDER(handle);

                UAssert(render->ID_ == ID_RENDERABLE, "Attempted to get renderable from non-valid handle!");
                UAssert(render->Type_ == RENDERABLE_LIGHT, "Attempted to use invalid handle to get a billboard2D!");

                return m_renderableLights[ render->Index_ ];
            }

        private:
            void CacheFrame(void);

        private:
            //all the renderables
            std::vector<Model3D> m_currentRenderableModel3D;
            std::vector<Primitive> m_currentRenderablePrimitives;
            std::vector<Billboard2D> m_currentRenderableBillboards;
            std::vector<Light> m_currentRenderableLights;

            //cahced data used to render frame N while updating frame N + 1
            std::vector<Model3D> m_renderableModel3D;
            std::vector<Primitive> m_renderablePrimitives;
            std::vector<Billboard2D> m_renderableBillboards;
            std::vector<Light> m_renderableLights;

            //all the free handles
            std::vector<std::list<unsigned>*> m_handleList;
        };
    }
}

#include "RenderableManager.hpp"
