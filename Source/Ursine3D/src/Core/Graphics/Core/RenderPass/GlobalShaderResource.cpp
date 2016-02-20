/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** GlobalShaderResource.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "GlobalShaderResource.h"
#include "GfxManager.h"
#include "ShaderBufferList.h"

namespace ursine
{
    namespace graphics
    {
         GfxManager *GlobalShaderResource::m_manager;

         static char *slotNames[ 15 ] = {
             "SHADER_SLOT_0",
             "SHADER_SLOT_1",
             "SHADER_SLOT_2",
             "SHADER_SLOT_3",
             "SHADER_SLOT_4",
             "SHADER_SLOT_5",
             "SHADER_SLOT_6",
             "SHADER_SLOT_7",
             "SHADER_SLOT_8",
             "SHADER_SLOT_9",
             "SHADER_SLOT_10",
             "SHADER_SLOT_11",
             "SHADER_SLOT_12",
             "SHADER_SLOT_13",
             "SHADER_SLOT_14"
         };

         static char *resourceTypeNames[6] = {
             "RESOURCE_CBUFFER",
             "RESOURCE_MODEL",         
             "RESOURCE_TEXTURE",       
             "RESOURCE_INPUT_RT",      
             "RESOURCE_INPUT_DEPTH",                  
             "RESOURCE_COUNT"
         };

        ///////////////////////////////////////////////////////////////////////
        GlobalShaderResource::GlobalShaderResource(
            ShaderSlot slotIndex, 
            ResourceType resourceType,
            SHADERTYPE targetShader
        )
            : m_slotIndex( slotIndex)
            , m_resourceType( resourceType )
            , m_targetShader( targetShader )
        {
        }

        void GlobalShaderResource::Map()
        {
        }

        GlobalShaderResource::~GlobalShaderResource()
        {
        }

        bool GlobalShaderResource::operator=(const GlobalShaderResource & rhs) const
        {
            // if we collide on shader, resource, and index, return true
            return (m_slotIndex == rhs.m_slotIndex) &&
                   (m_resourceType == rhs.m_resourceType) && 
                   (m_targetShader == rhs.m_targetShader);
        }

        void GlobalShaderResource::SetGfxMgr(GfxManager * mgr)
        {
            m_manager = mgr;
        }

        ///////////////////////////////////////////////////////////////////////
        GlobalGPUResource::GlobalGPUResource(
            ShaderSlot shaderSlot, 
            ResourceType type
        )
            : GlobalShaderResource(shaderSlot, type, SHADERTYPE_COUNT)
            , m_resourceID( -1 )
        {
        }

        GlobalGPUResource::~GlobalGPUResource() { }

        void GlobalGPUResource::Update(unsigned resourceID)
        {
            m_resourceID = resourceID;
        }

        void GlobalGPUResource::Map(void)
        {
            UAssert( m_resourceID != -1, "%s bound to %s never given data!" , 
                resourceTypeNames[m_resourceType], 
                slotNames[m_slotIndex]
            );

            switch( m_resourceType )
            {
            case RESOURCE_MODEL:
                m_manager->modelManager->BindModel( m_resourceID, m_slotIndex );
                break;
            case RESOURCE_TEXTURE:
                m_manager->textureManager->MapTextureByID( m_resourceID, m_slotIndex );
                break;
            case RESOURCE_INPUT_RT:
            {
                // grab the rt texture
                auto *rt = m_manager->dxCore->GetRenderTargetMgr( )->GetRenderTarget( 
                    static_cast<RENDER_TARGETS>( m_resourceID ) )->ShaderMap;

                // map it on gpu
                m_manager->dxCore->GetDeviceContext( )->PSSetShaderResources( m_slotIndex, 1, &rt );
            }
            break;
            case RESOURCE_INPUT_DEPTH:
            {
                // grab the depth texture
                auto *srv = m_manager->dxCore->GetDepthMgr( )->GetDepthStencilSRV(
                    static_cast<DEPTH_STENCIL_LIST>( m_resourceID )
                );

                // map to gpu
                m_manager->dxCore->GetDeviceContext( )->PSSetShaderResources( m_slotIndex, 1, &srv );
            }
                break;
            default:
                break;
            }
        }
    }
}