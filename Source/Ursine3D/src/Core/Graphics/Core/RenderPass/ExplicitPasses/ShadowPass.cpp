/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ShadowPass.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ShadowPass.h"
#include "GfxManager.h"

namespace ursine
{
    namespace graphics
    {
        ShadowPass::ShadowPass(const std::string &passName)
            : RenderPass(passName)
        { }

        ShadowPass::~ShadowPass() { }

        void ShadowPass::executePass(Camera &currentCamera)
        {
            D3D11_VIEWPORT vpData;
            vpData.TopLeftX = 0;
            vpData.TopLeftY = 0;
            vpData.MinDepth = 0;
            vpData.MaxDepth = 1;

            /////////////////////////////////////////////////////////////////////////////
            // GET OBJECT RANGE
            unsigned objStart = 0, objEnd = 0;
            auto &drawList = m_manager->m_drawList;

            // get the start of objects
            while (drawList[ objStart ].Type_ != RENDERABLE_MODEL3D && objStart < m_manager->m_drawCount)
                ++objStart;
            objEnd = objStart;

            // get the end of objects
            while (drawList[ objEnd ].Type_ == RENDERABLE_MODEL3D && objEnd < m_manager->m_drawCount)
                ++objEnd;

            /////////////////////////////////////////////////////////////////////////////
            // GET LIGHT RANGE
            unsigned lightStart = 0, lightEnd = 0;

            // get the start of objects
            while (drawList[ lightStart ].Type_ != m_renderableMode && lightStart < m_manager->m_drawCount)
                ++lightStart;
            lightEnd = lightStart;

            // get the end of objects
            while (drawList[ lightEnd ].Type_ == m_renderableMode && lightEnd < m_manager->m_drawCount)
                ++lightEnd;

            // start debug event
            m_manager->dxCore->StartDebugEvent(m_passName);

            /////////////////////////////////////////////////////////////////////////////
            // CREATE SHADOW MAPS

            // for each light,
            for(unsigned x = lightStart; x < lightEnd; ++x)
            {
                // get light
                Light &light = m_manager->renderableManager->GetRenderableByID<Light>(drawList[ x ].Index_);

                if ( light.GetType() != Light::LIGHT_SPOTLIGHT )
                    continue;

                if(!light.GetRenderShadows( ))
                    continue;

                m_manager->GetDXCore( )->StartDebugEvent( "Shadow Start" );

                beginPass( currentCamera );

                setDepthTarget( &light );

                vpData.Width    = static_cast<float>( light.GetShadowmapWidth( ) );
                vpData.Height   = static_cast<float>( light.GetShadowmapWidth( ) );

                m_manager->dxCore->GetDeviceContext( )->RSSetViewports(
                    1,
                    &vpData
                );

                // set light data
                auto view       = light.GenerateShadowView( );
                auto proj       = light.GenerateShadowProjection( );
                
                // map shadow view/projection
                m_manager->bufferManager->MapCameraBuffer( 
                    view, 
                    proj 
                );

                // render this set of objects
                m_processor->Process(
                    m_manager->m_drawList,
                    currentCamera,
                    objStart,
                    objEnd
                );

                m_manager->GetDXCore( )->StartDebugEvent("Shadow End");
            }

            // end debug event
            m_manager->dxCore->EndDebugEvent();

            // stamp this process
            m_manager->gfxProfiler->Stamp(m_passName);
        }

        void ShadowPass::setDepthTarget(Light *light)
        {
            auto dxCore     = m_manager->GetDXCore( );
            auto devCon     = dxCore->GetDeviceContext( );
            auto depthMgr   = dxCore->GetDepthMgr( );

            // if it doesn't have a light or needs to be resized, do so
            auto handle     = light->GetShadowmapHandle( );
            auto width      = light->GetShadowmapWidth( );

            // if it exists, check for resize
            if(handle != 0)
            {
                auto depthStencil = depthMgr->GetShadowmapDepthStencil( light->GetShadowmapHandle( ) );

                if(width != depthStencil.width)
                    depthMgr->ResizeShadowmapDepthTarget( 
                        handle, 
                        width, 
                        width 
                    );
            }
            else // create a new one
            {
                light->SetShadowmapHandle(
                    depthMgr->CreateShadowmapDepthTarget(
                        width, 
                        width
                    )
                );
            }

            auto depthStencil = depthMgr->GetShadowmapDepthStencil( light->GetShadowmapHandle( ) );

            // clear depth
            m_manager->GetDXCore( )->GetDeviceContext( )->ClearDepthStencilView(
                depthStencil.depthStencilView, 
                D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 
                1.0f, 
                0
            );

            // get RTs
            for (unsigned x = 0; x < m_outputTargets.size(); ++x)
            {
                m_targetArray[ x ] = dxCore->GetRenderTargetMgr()->GetRenderTarget( m_outputTargets[ x ] )->RenderTargetView;
            }

            // Set RTs
            devCon->OMSetRenderTargets(
                static_cast<UINT>( m_outputTargets.size( ) ),
                m_targetArray.data( ),
                depthStencil.depthStencilView
            );
        }
    }
}