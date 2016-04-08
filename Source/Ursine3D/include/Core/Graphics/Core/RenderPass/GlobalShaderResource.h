/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** GlobalShaderResource.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    namespace graphics
    {
        class GfxManager;
        enum SHADERTYPE : unsigned;
        enum BUFFER_LIST : unsigned;
        
        enum ResourceType : unsigned
        {
            RESOURCE_CBUFFER        = 0,
            RESOURCE_MODEL          = 1,                // vert and index buff as a models
            RESOURCE_TEXTURE        = 66,    // texture buffer
            RESOURCE_INPUT_RT       = 68,    // render target as texture
            RESOURCE_INPUT_DEPTH    = 76,    // depth target as texture
            RESOURCE_SHADOWMAP      = 16,
            RESOURCE_COUNT          = 7
        };

        enum ShaderSlot : unsigned
        {
            SHADER_SLOT_0,
            SHADER_SLOT_1,
            SHADER_SLOT_2,
            SHADER_SLOT_3,
            SHADER_SLOT_4,
            SHADER_SLOT_5,
            SHADER_SLOT_6,
            SHADER_SLOT_7,
            SHADER_SLOT_8,
            SHADER_SLOT_9,
            SHADER_SLOT_10,
            SHADER_SLOT_11,
            SHADER_SLOT_12,
            SHADER_SLOT_13,

            SHADER_SLOT_COUNT
        };
        
        ///////////////////////////////////////////////////////////////////////
        // main class for global resources
        class GlobalShaderResource
        {
        public:
            GlobalShaderResource(ShaderSlot slotIndex, ResourceType resourceType, SHADERTYPE targetShader);

            // map this resource to the GPU in a specified slot
            virtual void Map(void);
            virtual ~GlobalShaderResource(void);

            bool operator=(const GlobalShaderResource &rhs) const;
            ShaderSlot GetSlotIndex(void) const;
            static void SetGfxMgr(GfxManager *mgr);

        protected:
            static GfxManager *m_manager;   // the manager

            ShaderSlot m_slotIndex;         // where to map
            ResourceType m_resourceType;    // what resource is this?
            SHADERTYPE m_targetShader;      // what shader expects this resource
        };

        ///////////////////////////////////////////////////////////////////////
        // class for a global constant buffer
        template<typename T, BUFFER_LIST BufferType>
        class GlobalCBuffer : public GlobalShaderResource
        {
        public:
            GlobalCBuffer(SHADERTYPE shader, ShaderSlot shaderSlotIndex = static_cast<ShaderSlot>(BufferType));
            virtual ~GlobalCBuffer(void);

            // update this data to prepare for GPU usage
            void Update(const T &data, ShaderSlot slotIndex = SHADER_SLOT_COUNT);

            // map data
            void Map(void) override;

        private:
            T m_bufferData;
        };

        ///////////////////////////////////////////////////////////////////////
        // class for any other resource
        class GlobalGPUResource : public GlobalShaderResource
        {
        public:
            GlobalGPUResource(ShaderSlot shaderSlot, ResourceType type, SHADERTYPE shadertype = static_cast<SHADERTYPE>(6));
            virtual ~GlobalGPUResource(void);

            // update whatever is on the gpu
            void Update(unsigned resourceID);

            // map data
            void Map(void) override;

        private:
            int m_resourceID;
        };

        #include "GlobalShaderResource.hpp"
    }
}