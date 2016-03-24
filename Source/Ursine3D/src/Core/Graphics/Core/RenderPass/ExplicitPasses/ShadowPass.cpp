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
            vpData.Width = 1024;
            vpData.Height = 1024;
            vpData.TopLeftX = 0;
            vpData.TopLeftY = 0;
            vpData.MinDepth = 0;
            vpData.MaxDepth = 1;

            m_manager->dxCore->GetDeviceContext()->RSSetViewports(
                1,
                &vpData
            );

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
                beginPass( currentCamera );

                m_manager->dxCore->GetDeviceContext()->RSSetViewports(
                    1,
                    &vpData
                );

                // get light
                Light &light = m_manager->renderableManager->GetRenderableByID<Light>( drawList[ x ].Index_ );

                if(light.GetType( ) != Light::LIGHT_SPOTLIGHT)
                    continue;

                // set light data
                auto view = light.GenerateShadowView( );
                auto proj = light.GenerateShadowProjection( );
                auto invCam = currentCamera.GetViewMatrix( );
                invCam.Inverse( );
                
                // map shadow view/projection
                m_manager->bufferManager->MapCameraBuffer( 
                    view, 
                    proj 
                );

                ShadowProjectionBuffer spb;
                invCam.Transpose( );
                view.Transpose( );
                proj.Transpose( );
                spb.invCam      = invCam.ToD3D( );
                spb.lightView   = view.ToD3D( );
                spb.lightProj   = proj.ToD3D( );

                m_manager->bufferManager->MapBuffer<BUFFER_SHADOWMAP, ShadowProjectionBuffer>(
                    &spb, 
                    SHADERTYPE_PIXEL, 
                    13
                );

                // render this set of objects
                m_processor->Process(
                    m_manager->m_drawList,
                    currentCamera,
                    objStart,
                    objEnd
                );
            }

            // end debug event
            m_manager->dxCore->EndDebugEvent();

            // stamp this process
            m_manager->gfxProfiler->Stamp(m_passName);
        }
    }
}