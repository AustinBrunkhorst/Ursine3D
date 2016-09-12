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
        void RenderableManager::Initialize(GfxManager *manager)
        {
            m_currentRenderableModel3D.resize(MAX_RENDERABLES);
            m_currentRenderableBillboards.resize(MAX_RENDERABLES);
            m_currentRenderableLights.resize(MAX_RENDERABLES);
            m_currentRenderableParticleSystems.resize(MAX_RENDERABLES);
            m_currentRenderableSpriteText.resize(MAX_RENDERABLES);

            m_renderableModel3D.resize(MAX_RENDERABLES);
            m_renderableBillboards.resize(MAX_RENDERABLES);
            m_renderableLights.resize(MAX_RENDERABLES);
            m_renderableParticleSystems.resize(MAX_RENDERABLES);
            m_renderableSpriteText.resize(MAX_RENDERABLES);

            m_handleList.resize(RENDERABLE_TYPE_COUNT);

            m_manager = manager;

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
                m_currentRenderableModel3D[ newRender->Index_ ].m_active = true;
                m_currentRenderableModel3D[ newRender->Index_ ].Initialize( );
                break;
            case RENDERABLE_LIGHT:
                m_currentRenderableLights[ newRender->Index_ ].m_active = true;
                m_currentRenderableLights[ newRender->Index_ ].Initialize( );
                break;
            case RENDERABLE_BILLBOARD2D:
                m_currentRenderableBillboards[ newRender->Index_ ].m_active = true;
                m_currentRenderableBillboards[ newRender->Index_ ].Initialize();
                break;
            case RENDERABLE_PS:
                m_currentRenderableParticleSystems[ newRender->Index_ ].m_active = true;
                m_currentRenderableParticleSystems[ newRender->Index_ ].Initialize();
                break;
            case RENDERABLE_SPRITE_TEXT:
                m_currentRenderableSpriteText[ newRender->Index_ ].m_active = true;
                m_currentRenderableSpriteText[ newRender->Index_ ].Initialize();
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

            switch (rend->Type_)
            {
            case RENDERABLE_MODEL3D:
                m_currentRenderableModel3D[ rend->Index_ ].Uninitialize( m_manager );
                break;
            case RENDERABLE_LIGHT:
                m_currentRenderableLights[ rend->Index_ ].Uninitialize( m_manager );
                break;
            case RENDERABLE_BILLBOARD2D:
                m_currentRenderableBillboards[ rend->Index_ ].Uninitialize( m_manager );
                break;
            case RENDERABLE_PS:
                m_currentRenderableParticleSystems[ rend->Index_ ].Uninitialize( m_manager );
                break;
            case RENDERABLE_SPRITE_TEXT:
                m_currentRenderableSpriteText[ rend->Index_ ].Uninitialize( m_manager );
                break;
            default:
                UAssert(false, "Tried to destroy an invalid renderable!");
            }

            m_handleList[ rend->Type_ ].push(rend->Index_);

            handle = 0;
        }

        void RenderableManager::CacheFrame(void)
        {
            m_renderableModel3D             = m_currentRenderableModel3D;
            m_renderableBillboards          = m_currentRenderableBillboards;
            m_renderableLights              = m_currentRenderableLights;
            m_renderableParticleSystems     = m_currentRenderableParticleSystems;
            m_renderableSpriteText          = m_currentRenderableSpriteText;
        }   
    }
}
