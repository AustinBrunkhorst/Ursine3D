/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** RenderPass.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "RenderPass.h"
#include "GfxManager.h"
#include "GlobalShaderResource.h"
#include "GraphicsEntityProcessor.h"

namespace ursine
{
    namespace graphics
    {
        GfxManager *RenderPass::m_manager = nullptr;

        RenderPass::RenderPass(
            std::string name
        )
            : m_prePasses( )
            , m_postPasses( )
            , m_outputTargets( )
            , m_targetArray( )
            , m_shader( SHADER_COUNT )
            , m_inputLayout( SHADER_COUNT )
            , m_depthStencil( DEPTH_STENCIL_COUNT )
            , m_depthState( DEPTH_STATE_COUNT )
            , m_samplerStates( )
            , m_rasterState( RASTER_STATE_COUNT )
            , m_blendState( BLEND_STATE_COUNT )
            , m_topology( DXCore::TOPOLOGY_COUNT )
            , m_renderableMode( RENDERABLE_TYPE_COUNT )
            , m_processor( nullptr )
            , m_globalResources( )
            , m_passName( name )
            , m_explicitOverride( false )
            , m_fullscreenPass( false )
        {
        }

        void RenderPass::InitializePass(void)
        {
            // we need to do all of our safety checking to make sure a pass is valid
            // need more checks to validate

            // no shaders
            if(m_shader == SHADER_COUNT)
            {
                // also no passes? that means there's nothing it can do
                UAssert( m_prePasses.size( ) + m_postPasses.size( ) > 0, "%s did not have shaders or sub passes!", m_passName.c_str( ) );

                // if we have no shader we can't process objects
                UAssert( m_renderableMode == RENDERABLE_TYPE_COUNT, "%s accepts entities without having a shader!", m_passName.c_str( ) );
            }

            // we accept some type of object
            if(m_renderableMode != RENDERABLE_TYPE_COUNT)
            {
                // entities, no targets, no depth? throw
                if (m_outputTargets.size( ) == 0)
                    UAssert( m_depthStencil != DEPTH_STENCIL_COUNT, "%s attempted to render output without targets!", m_passName.c_str( ) );
                
                UAssert( m_processor != nullptr, "%s renders entities but had no processor!", m_passName.c_str( ) );
            }

            // if we provide no passes and no fullscreen pass
            if(m_prePasses.size( ) + m_postPasses.size( ) == 0 && m_fullscreenPass == false)
            {
                // we better have an entity
                UAssert( m_renderableMode != RENDERABLE_TYPE_COUNT, "%s has no entities or pass to work with!", m_passName.c_str( ) );
            }

            // check for conflicts on bound global resource slots
            auto globalResourceSize = m_globalResources.size( );
            for(size_t x = 0; x < globalResourceSize; ++x)
                for( size_t y = x + 1; y < globalResourceSize; ++y)
                    UAssert( m_globalResources[ x ] != m_globalResources[ y ], "%s has conflicting global resources (Resource indices: %i, %i. Conflicting index: %i)", 
                        m_passName.c_str( ), 
                        x, 
                        y,
                        m_globalResources[ x ]->GetSlotIndex( ) 
                    );

            // if a fullscreen pass, we shouldn't have entities or whatever
            if(m_fullscreenPass)
            {
                UAssert( m_processor == nullptr, "% was given a processor, but it is a fullscreen pass!", m_passName.c_str( ) );
                UAssert( m_renderableMode == RENDERABLE_TYPE_COUNT, "%s was given an entity to process, but it is a fullscreen pass!", m_passName.c_str( ) );
            }
            
            // make sure stuff isn't bound on input and output

            // resize targets
            m_targetArray.resize( m_outputTargets.size( ) );
        }

        void RenderPass::Execute(Camera &currentCamera)
        {
            // trigger pre-passes
            executePrePasses( currentCamera );

            // begin our pass
            beginPass( currentCamera );

            // execute our own pass
            executePass( currentCamera );

            // triger post-passes
            executePostPasses( currentCamera );
        }

        RenderPass &RenderPass::Set(std::initializer_list<RENDER_TARGETS> targets)
        {
            for ( auto target : targets )
            {
                m_outputTargets.push_back( target );
            }
            return *this;
        }

        RenderPass &RenderPass::Set(SHADER_LIST shader)
        {
            m_shader = shader;
            return *this;
        }

        RenderPass &RenderPass::Set(DEPTH_STENCIL_LIST depthStencil)
        {
            m_depthStencil = depthStencil;
            return *this;
        }

        RenderPass &RenderPass::Set(DEPTH_STATES depthState)
        {
            m_depthState = depthState;
            return *this;
        }

        RenderPass &RenderPass::Set(SAMPLER_STATES samplerState, unsigned index)
        {
            if(m_samplerStates.size( ) < index + 1)
            {
                m_samplerStates.resize( index + 1 );
            }

            m_samplerStates[ index ] = samplerState;

            return *this;
        }

        RenderPass &RenderPass::Set(RASTER_STATES rasterState)
        {
            m_rasterState = rasterState;
            return *this;
        }

        RenderPass &RenderPass::Set(BLEND_STATES blendState)
        {
            m_blendState = blendState;
            return *this;
        }

        RenderPass &RenderPass::Set(DXCore::PRIMITIVE_TOPOLOGY topology)
        {
            m_topology = topology;
            return *this;
        }

        RenderPass &RenderPass::Accepts(RENDERABLE_TYPE type)
        {
            m_renderableMode = type;
            return *this;
        }

        RenderPass &RenderPass::Processes(GraphicsEntityProcessor *processor)
        {
            m_processor = processor;
            return *this;
        }

        RenderPass &RenderPass::AddResource(GlobalShaderResource *gsr)
        {
            m_globalResources.push_back( gsr );
            return *this;
        }

        RenderPass &RenderPass::AddPrePass(RenderPass *newPass)
        {
            m_prePasses.push_back( newPass );
            return *this;
        }

        RenderPass &RenderPass::AddPostPass(RenderPass *newPass)
        {
            m_postPasses.push_back( newPass );
            return *this;
        }

        RenderPass &RenderPass::OverrideLayout(SHADER_LIST inputLayout)
        {
            m_inputLayout = inputLayout;
            return *this;
        }

        RenderPass &RenderPass::IsFullscreenPass(bool isFullscreenPass)
        {
            m_fullscreenPass = isFullscreenPass;
            return *this;
        }

        void RenderPass::SetGfxMgr(GfxManager * mgr)
        {
            m_manager = mgr;
        }

        void RenderPass::beginPass(Camera &currentCamera)
        {
            setGPUState( );
            mapGlobals( );

            //set directx viewport
            Viewport &gameVP = m_manager->viewportManager->GetViewport(m_manager->m_GameViewport);
            D3D11_VIEWPORT gvp = gameVP.GetViewportData();
            
            float w, h, x, y;
            currentCamera.GetViewportPosition( x, y );
            currentCamera.GetDimensions( w, h );

            w *= gvp.Width;
            h *= gvp.Height;

            x = x * gvp.Width + gvp.TopLeftX;
            y = y * gvp.Height + gvp.TopLeftY;

            D3D11_VIEWPORT vpData;
            vpData.Width = w;
            vpData.Height = h;
            vpData.TopLeftX = x;
            vpData.TopLeftY = y;
            vpData.MinDepth = 0;
            vpData.MaxDepth = 1;

            m_manager->dxCore->GetDeviceContext()->RSSetViewports(
                1, 
                &vpData 
            );
        }

        void RenderPass::executePrePasses(Camera &currentCamera)
        {
            for (auto &pass : m_prePasses)
                pass->Execute( currentCamera );
        }

        void RenderPass::executePass(Camera &currentCamera)
        {
            // check to see if this pass has stuff to do
            if (m_processor != nullptr && m_fullscreenPass == false)
            {
                // calculate our range
                unsigned start = 0, end = 0;
                auto &drawList = m_manager->m_drawList;

                // get the start
                while(drawList[start].Type_ != m_renderableMode && start < m_manager->m_drawCount)
                    ++start;
                end = start;

                // get the end
                while(drawList[end].Type_ == m_renderableMode && end < m_manager->m_drawCount )
                    ++end;

                // start debug event
                m_manager->dxCore->StartDebugEvent( m_passName );

                // process
                m_processor->Process( 
                    m_manager->m_drawList, 
                    currentCamera, 
                    start, 
                    end 
                );

                // end debug event
                m_manager->dxCore->EndDebugEvent( );

                // stamp this process
                m_manager->gfxProfiler->Stamp( m_passName );
            }
            else if(m_fullscreenPass)
            {
                m_manager->shaderManager->Render( 6 );
            }
        }

        void RenderPass::executePostPasses(Camera &currentCamera)
        {
            for (auto &pass : m_postPasses)
                pass->Execute( currentCamera );
        }

        void RenderPass::setGPUState(void)
        {
            auto dxCore = m_manager->GetDXCore( );
            auto devCon = dxCore->GetDeviceContext( );

            // get depth target
            auto *depthTarget = dxCore->GetDepthMgr( )->GetDepthStencilView( m_depthStencil );

            // get RTs
            for (unsigned x = 0; x < m_outputTargets.size( ); ++x)
            {
                m_targetArray[ x ] = dxCore->GetRenderTargetMgr( )->GetRenderTarget( m_outputTargets[ x ] )->RenderTargetView;
            }

            // Set RTs
            devCon->OMSetRenderTargets(
                static_cast<UINT>( m_outputTargets.size( ) ), 
                m_targetArray.data( ), 
                depthTarget 
            );

            // bind shader
            m_manager->shaderManager->BindShader( m_shader );

            // bind layout to match shader. If we overrode it, use that instead
            if (m_inputLayout == SHADER_COUNT)
                m_manager->layoutManager->SetInputLayout( m_shader );
            else
                m_manager->layoutManager->SetInputLayout( m_inputLayout );

            // set depth stencil state
            dxCore->SetDepthState( m_depthState );

            // map sampler
            for(int x = 0; x < m_samplerStates.size( ); ++x)
            {
                m_manager->textureManager->MapSamplerState(m_samplerStates[ x ], x);
            }
            

            // map raster 
            dxCore->SetRasterState( m_rasterState );

            // map blending
            dxCore->SetBlendState( m_blendState );

            // map topology
            devCon->IASetPrimitiveTopology( static_cast<D3D_PRIMITIVE_TOPOLOGY>(m_topology) );
        }

        void RenderPass::mapGlobals(void)
        {
            for (auto &x : m_globalResources)
                x->Map( );
        }
    }
}
