/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** RenderPass.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "D3D11Forward.h"
#include "GlobalShaderResource.h"
#include "GraphicsEntityProcessor.h"

namespace ursine
{
    namespace graphics
    {
        // forward declare all enums
        enum SHADER_LIST           : unsigned;
        enum COMPUTE_BUFFER_LIST    : unsigned;
        enum RENDER_TARGETS         : unsigned;
        enum RASTER_STATES          : unsigned;
        enum DEPTH_STATES           : unsigned;
        enum DEPTH_STENCIL_LIST     : unsigned;
        enum BLEND_STATES           : unsigned;
        enum SAMPLER_STATES         : unsigned;
        enum RENDERABLE_TYPE        : unsigned;

        namespace DXCore
        {
            enum PRIMITIVE_TOPOLOGY : unsigned;
        }
        
        // forward declare classes
        class GlobalShaderResource;
        class GraphicsEntityProcessor;
        class GfxManager;

        // a render pass in the engine
        class RenderPass
        {
        public:
            RenderPass(std::string name = "UNNAMED PASS");

            // static initialization for object
            // checks for problems with a pass
            void InitializePass(void);

            // execute this pass
            void Execute(Camera &currentCamera);

            // all the sets
            RenderPass &Set(std::initializer_list<RENDER_TARGETS> targets);     // where do we write to
            RenderPass &Set(SHADER_LIST shader);                                // shader we use
            RenderPass &Set(DEPTH_STENCIL_LIST depthStencil);                   // what depth we read from
            RenderPass &Set(DEPTH_STATES depthState);                           // what depth state
            RenderPass &Set(SAMPLER_STATES samplerState, unsigned index = 0);   // how do we sample
            RenderPass &Set(RASTER_STATES rasterState);                         // how do we rasterize
            RenderPass &Set(BLEND_STATES blendState);                           // how do we blend
            RenderPass &Set(DXCore::PRIMITIVE_TOPOLOGY topology);               // topology

            RenderPass &Accepts(RENDERABLE_TYPE type);                          // what will we accept?
            RenderPass &Processes(GraphicsEntityProcessor *processor);          // what will we use to process?
            RenderPass &AddResource(GlobalShaderResource *gsr);                 // global resources
            RenderPass &AddPrePass(RenderPass *newPass);                        // sub passes
            RenderPass &AddPostPass(RenderPass *newPass);                       // sub passes

            RenderPass &OverrideLayout(SHADER_LIST inputLayout);                // override the input layout
            RenderPass &IsFullscreenPass(bool isFullscreenPass);                // sets as a fullscreen pass

            static void SetGfxMgr(GfxManager *mgr);                             // setting up mgr

        protected:
            // dynamic initialization for this frame
            // update global resources
            virtual void beginPass(Camera &currentCamera);

            // process pre-passes
            void executePrePasses(Camera &currentCamera);

            // process self, can be overridden for extremely specific purposes
            virtual void executePass(Camera &currentCamera);

            // process post-passes
            void executePostPasses(Camera &currentCamera);

            // set current state of GPU to desired state
            void setGPUState(void);

            // map all globals
            void mapGlobals(void);

        protected:
            // the manager
            static GfxManager *m_manager;

            // passes that will be called within this pass
            std::vector<RenderPass *> m_prePasses;
            std::vector<RenderPass *> m_postPasses;

            // where we render to
            std::vector<RENDER_TARGETS> m_outputTargets;
            std::vector<ID3D11RenderTargetView*> m_targetArray;

            // shader, tied to input layout?
            SHADER_LIST m_shader;

            // input layout to use. will automatically use shader unless overridden
            SHADER_LIST m_inputLayout;

            // depth target
            DEPTH_STENCIL_LIST m_depthStencil;

            // depth state
            DEPTH_STATES m_depthState;

            // sampler
            std::vector<SAMPLER_STATES> m_samplerStates;

            // raster state
            RASTER_STATES m_rasterState;

            // blend state
            BLEND_STATES m_blendState;

            // topology state
            DXCore::PRIMITIVE_TOPOLOGY m_topology;

            // what object we expect to work with?
                // will be used to request handles
            RENDERABLE_TYPE m_renderableMode;

            // how we handle objects
            GraphicsEntityProcessor *m_processor;

            // "global" resources, static to this pass (like cam buffer, textures, etc)
            std::vector<GlobalShaderResource*> m_globalResources;

            // name of this pass
            std::string m_passName;

            // special override for explicit passes
            bool m_explicitOverride;

            // is this a once-over fullscreen pass?
            bool m_fullscreenPass;
        };
    }
}

