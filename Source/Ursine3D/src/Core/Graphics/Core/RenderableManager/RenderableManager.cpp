#include "UrsinePrecompiled.h"
#include "RenderableManager.h"

namespace ursine
{
    namespace graphics
    {
        void RenderableManager::Initialize(void)
        {
            m_currentRenderableModel3D.resize(MAX_RENDERABLES);
            m_currentRenderableDirectionalLight.resize(MAX_RENDERABLES);
            m_currentRenderablePointLight.resize(MAX_RENDERABLES);
            m_currentRenderablePrimitives.resize(MAX_RENDERABLES);
            m_currentRenderableBillboards.resize(MAX_RENDERABLES);
            m_currentRenderableLights.resize(MAX_RENDERABLES);

            m_renderableModel3D.resize(MAX_RENDERABLES);
            m_renderableDirectionalLight.resize(MAX_RENDERABLES);
            m_renderablePointLight.resize(MAX_RENDERABLES);
            m_renderablePrimitives.resize(MAX_RENDERABLES);
            m_renderableBillboards.resize(MAX_RENDERABLES);
            m_renderableLights.resize(MAX_RENDERABLES);

            m_handleList.resize(RENDERABLE_TYPE_COUNT);

            for (unsigned x = 0; x < RENDERABLE_TYPE_COUNT; ++x)
            {
                m_handleList[ x ] = new std::list<unsigned>();
                for (int y = 0; y < MAX_RENDERABLES; ++y)
                    m_handleList[ x ]->push_back(y);
            }
        }

        void RenderableManager::Uninitialize()
        {
            for (unsigned x = 0; x < RENDERABLE_TYPE_COUNT; ++x)
            {
                delete m_handleList[ x ];
            }
        }

        GfxHND RenderableManager::AddRenderable(const RENDERABLE_TYPE type)
        {
            GfxHND data;
            _RENDERABLEHND *newRender = reinterpret_cast<_RENDERABLEHND*>(&data);
            newRender->Index_ = m_handleList[ type ]->front();
            m_handleList[ type ]->pop_front();

            switch (type)
            {
            case RENDERABLE_MODEL3D:
                m_currentRenderableModel3D[ newRender->Index_ ].Active_ = true;
                m_currentRenderableModel3D[ newRender->Index_ ].Initialize( );
                break;
            case RENDERABLE_DIRECTION_LIGHT:
                m_currentRenderableDirectionalLight[ newRender->Index_ ].Active_ = true;
                m_currentRenderableDirectionalLight[ newRender->Index_ ].Initialize( );
                break;
            case RENDERABLE_POINT_LIGHT:
                m_currentRenderablePointLight[ newRender->Index_ ].Active_ = true;
                m_currentRenderablePointLight[ newRender->Index_ ].Initialize( );
                break;
            case RENDERABLE_PRIMITIVE:
                m_currentRenderablePrimitives[ newRender->Index_ ].Active_ = true;
                m_currentRenderablePrimitives[ newRender->Index_ ].Initialize( );
                break;
            case RENDERABLE_LIGHT:
                m_currentRenderableLights[ newRender->Index_ ].Active_ = true;
                m_currentRenderableLights[ newRender->Index_ ].Initialize( );
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

            m_handleList[ rend->Type_ ]->push_front(rend->Index_);

            switch (rend->Type_)
            {
            case RENDERABLE_MODEL3D:
                UAssert(m_currentRenderableModel3D[ rend->Index_ ].Active_ == true, "Attempted to free an already free model3d!");
                m_currentRenderableModel3D[ rend->Index_ ].Active_ = false;
                break;
            case RENDERABLE_DIRECTION_LIGHT:
                UAssert(m_currentRenderableDirectionalLight[ rend->Index_ ].Active_ == true, "Attempted to free an already free directional light!");
                m_currentRenderableDirectionalLight[ rend->Index_ ].Active_ = false;
                break;
            case RENDERABLE_POINT_LIGHT:
                UAssert(m_currentRenderablePointLight[ rend->Index_ ].Active_ == true, "Attempted to free an already free point light!");
                m_currentRenderablePointLight[ rend->Index_ ].Active_ = false;
                break;
            case RENDERABLE_PRIMITIVE:
                UAssert(m_currentRenderablePrimitives[ rend->Index_ ].Active_ == true, "Attempted to free an already free primitive!");
                m_currentRenderablePrimitives[ rend->Index_ ].Active_ = false;
                break;
            case RENDERABLE_BILLBOARD2D:
                UAssert(m_currentRenderableBillboards[ rend->Index_ ].Active_ == true, "Attempted to free an already free billboard2d!");
                m_currentRenderableBillboards[ rend->Index_ ].Active_ = false;
            case RENDERABLE_LIGHT:
                UAssert(m_currentRenderableLights[ rend->Index_ ].Active_ == true, "Attempted to free an already free light!");
                m_currentRenderableLights[ rend->Index_ ].Active_ = false;
            default:
                UAssert(false, "Attempted to destroy corrupted handle!");
                break;
            }

            handle = 0;
        }

        void RenderableManager::CacheFrame(void)
        {
            m_renderableModel3D = m_currentRenderableModel3D;
            m_renderableDirectionalLight = m_currentRenderableDirectionalLight;
            m_renderablePointLight = m_currentRenderablePointLight;
            m_renderablePrimitives = m_currentRenderablePrimitives;
            m_renderableBillboards = m_currentRenderableBillboards;

            m_renderableLights = m_currentRenderableLights;
        }
    }
}
