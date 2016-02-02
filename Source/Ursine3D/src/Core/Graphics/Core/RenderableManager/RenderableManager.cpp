/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RenderableManager.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "RenderableManager.h"

namespace ursine
{
    namespace graphics
    {
        void RenderableManager::Initialize(void)
        {
            m_currentRenderableModel3D.resize(MAX_RENDERABLES);
            m_currentRenderableBillboards.resize(MAX_RENDERABLES);
            m_currentRenderableLights.resize(MAX_RENDERABLES);

            m_renderableModel3D.resize(MAX_RENDERABLES);
            m_renderableBillboards.resize(MAX_RENDERABLES);
            m_renderableLights.resize(MAX_RENDERABLES);
            m_renderableParticleSystems.resize(MAX_RENDERABLES);

            m_handleList.resize(RENDERABLE_TYPE_COUNT);

            for (unsigned x = 0; x < RENDERABLE_TYPE_COUNT; ++x)
            {
                for (int y = 0; y < MAX_RENDERABLES; ++y)
                    m_handleList[ x ].push(y);
            }
        }

        void RenderableManager::Uninitialize()
        {
        }

        GfxHND RenderableManager::AddRenderable(const RENDERABLE_TYPE type)
        {
            UAssert(m_handleList[ type ].size() > 0, "Out of room for renderables!");
            GfxHND data;
            _RENDERABLEHND *newRender = reinterpret_cast<_RENDERABLEHND*>(&data);
            newRender->Index_ = m_handleList[ type ].top( );
            m_handleList[ type ].pop( );

            switch (type)
            {
            case RENDERABLE_MODEL3D:
                m_renderableModel3D[ newRender->Index_ ].Active_ = true;
                m_renderableModel3D[ newRender->Index_ ].Initialize( );
                break;
            case RENDERABLE_LIGHT:
                m_renderableLights[ newRender->Index_ ].Active_ = true;
                m_renderableLights[ newRender->Index_ ].Initialize( );
                break;
            case RENDERABLE_BILLBOARD2D:
                m_renderableBillboards[ newRender->Index_ ].Active_ = true;
                m_renderableBillboards[ newRender->Index_ ].Initialize();
                break;
            case RENDERABLE_PS:
                m_renderableParticleSystems[ newRender->Index_ ].Active_ = true;
                m_renderableParticleSystems[ newRender->Index_ ].Initialize();
                break;
            default:
                UAssert(false, "Tried to add an invalid renderable!");
            }

            newRender->Type_ = type;
            newRender->ID_ = ID_RENDERABLE;

            return data;
        }

        void RenderableManager::DestroyRenderable(GfxHND &handle)
        {
            const _RENDERABLEHND *rend = reinterpret_cast<const _RENDERABLEHND*>(&handle);

            UAssert(rend->Index_ != ID_RENDERABLE, "attempted to free a non-valid renderable handle");

            m_handleList[ rend->Type_ ].push(rend->Index_);

            switch (rend->Type_)
            {
            case RENDERABLE_MODEL3D:
                UAssert(m_renderableModel3D[ rend->Index_ ].Active_ == true, "Attempted to free an already free model3d!");
				m_renderableModel3D[ rend->Index_ ].Active_ = false;
                break; 
            case RENDERABLE_BILLBOARD2D:
                UAssert(m_renderableBillboards[ rend->Index_ ].Active_ == true, "Attempted to free an already free billboard2d!");
                m_renderableBillboards[ rend->Index_ ].Active_ = false;
                break;
            case RENDERABLE_LIGHT:
                UAssert(m_renderableLights[ rend->Index_ ].Active_ == true, "Attempted to free an already free light!");
				m_renderableLights[ rend->Index_ ].Active_ = false;
                break;
            case RENDERABLE_PS:
                UAssert(m_renderableParticleSystems[ rend->Index_ ].Active_ == true, "Attempted to free an already free light!");
                m_renderableParticleSystems[ rend->Index_ ].Active_ = false;
                break;
            default:
                UAssert(false, "Attempted to destroy corrupted handle!");
                break;
            }

            handle = 0;
        }

        void RenderableManager::CacheFrame(void)
        {
            //m_renderableModel3D = m_currentRenderableModel3D;
            //m_renderablePrimitives = m_currentRenderablePrimitives;
            //m_renderableBillboards = m_currentRenderableBillboards;
            //m_renderableLights = m_currentRenderableLights;
        }
    }
}
