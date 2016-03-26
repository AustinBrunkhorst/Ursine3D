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

#pragma once

#include <vector>
#include <stack> 

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
            void Initialize(GfxManager *manager);
            void Uninitialize();

            GfxHND AddRenderable(const RENDERABLE_TYPE type);
            void DestroyRenderable(GfxHND &handle);

            template<typename T>
            T &GetRenderable(GfxHND handle)
            {
                UAssert(false, "Tried to get an invalid type!");
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

            template<>
            ParticleSystem &GetRenderable<ParticleSystem>(GfxHND handle)
            {
                _RENDERABLEHND *render = HND_RENDER(handle);

                UAssert(render->ID_ == ID_RENDERABLE, "Attempted to get renderable from non-valid handle!");
                UAssert(render->Type_ == RENDERABLE_PS, "Attempted to use invalid handle to get a particle system!");

                return m_renderableParticleSystems[ render->Index_ ];
            }

            template<>
            SpriteText &GetRenderable<SpriteText>(GfxHND handle)
            {
                _RENDERABLEHND *render = HND_RENDER(handle);

                UAssert(render->ID_ == ID_RENDERABLE, "Attempted to get renderable from non-valid handle!");
                UAssert(render->Type_ == RENDERABLE_SPRITE_TEXT, "Attempted to use invalid handle to get a sprite text!");

                return m_renderableSpriteText[ render->Index_ ];
            }

            template<typename T>
            T &GetRenderableByID(unsigned id)
            {
                UAssert(false, "Tried to get an invalid type!");
                return T();
            }

            template<>
            Model3D &GetRenderableByID<Model3D>(unsigned id)
            {
                return m_renderableModel3D[ id ];
            }

            template<>
            Billboard2D &GetRenderableByID<Billboard2D>(unsigned id)
            {
                return m_renderableBillboards[ id ];
            }

            template<>
            Light &GetRenderableByID<Light>(unsigned id)
            {
                return m_renderableLights[ id ];
            }

            template<>
            ParticleSystem &GetRenderableByID<ParticleSystem>(unsigned id)
            {
                return m_renderableParticleSystems[ id ];
            }

            template<>
            SpriteText &GetRenderableByID<SpriteText>(unsigned id)
            {
                return m_renderableSpriteText[ id ];
            }

        private:
            void CacheFrame(void);

        private:
            //all the renderables
            std::vector<Model3D> m_currentRenderableModel3D;
            std::vector<Billboard2D> m_currentRenderableBillboards;
            std::vector<Light> m_currentRenderableLights;
            std::vector<ParticleSystem> m_currentParticleSystems;

            //cahced data used to render frame N while updating frame N + 1
            std::vector<Model3D> m_renderableModel3D;
            std::vector<Billboard2D> m_renderableBillboards;
            std::vector<Light> m_renderableLights;
            std::vector<ParticleSystem> m_renderableParticleSystems;
            std::vector<SpriteText> m_renderableSpriteText;

            //all the free handles
            std::vector<std::stack<unsigned>> m_handleList;

            GfxManager *m_manager;
        };
    }
}

#include "RenderableManager.hpp"
