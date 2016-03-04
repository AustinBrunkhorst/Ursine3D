/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ResourceAPI.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "ResourceAPI.h"
#include "ModelManager.h"
#include "TextureManager.h"

namespace ursine
{
    namespace graphics
    {
        struct ResourceAPI::privData
        {
            ModelManager *modelMgr;
            TextureManager *textureMgr;
        };

        /////////////////////////////////////////////////////////
        // TEXTURE
        /////////////////////////////////////////////////////////
        //get texture handle

        GfxHND ResourceAPI::CreateDynamicTexture(const unsigned width, const unsigned height)
        {
            return m_privates->textureMgr->CreateDynamicTexture( width, height );
        }

        void ResourceAPI::ResizeDynamicTexture(GfxHND &handle, const unsigned width, const unsigned height)
        {
            m_privates->textureMgr->ResizeDynamicTexture( handle, width, height );
        }

        void ResourceAPI::DestroyDynamicTexture(GfxHND &handle)
        {
            m_privates->textureMgr->DestroyDynamicTexture( handle );
        }

        GfxHND ResourceAPI::CreateTexture(uint8_t *binaryData, size_t binarySize, unsigned width, unsigned height)
        {
            return m_privates->textureMgr->CreateTexture( binaryData, binarySize, width, height );
        }

        void ResourceAPI::DestroyTexture(GfxHND &handle)
        {
            m_privates->textureMgr->DestroyTexture( handle );
        }

        void ResourceAPI::LoadTexture(GfxHND handle)
        {
            m_privates->textureMgr->LoadTexture( handle );
        }

        void ResourceAPI::UnloadTexture(GfxHND handle)
        {
            m_privates->textureMgr->UnloadTexture( handle );
        }

        void ResourceAPI::GetBinaryInformation(GfxHND handle, uint8_t **dataPtr, size_t &binarySize)
        {
            m_privates->textureMgr->GetBinaryInformation( handle, dataPtr, binarySize );
        }

        /////////////////////////////////////////////////////////
        // MODEL
        /////////////////////////////////////////////////////////

        bool ResourceAPI::CheckModelExistence(const std::string &modelName)
        {
            return m_privates->modelMgr->CheckModelExistence(modelName);
        }

        GfxHND ResourceAPI::CreateModel(const ufmt_loader::ModelInfo &modelInfo)
        {
            return m_privates->modelMgr->CreateModel( modelInfo );
        }

        void ResourceAPI::DestroyModel(GfxHND &handle)
        {
            m_privates->modelMgr->DestroyModel( handle );
        }

        void ResourceAPI::LoadModel(GfxHND handle)
        {
            m_privates->modelMgr->LoadModel( handle );
        }

        void ResourceAPI::UnloadModel(GfxHND handle)
        {
            m_privates->modelMgr->UnloadModel( handle );
        }

        ModelResource *ResourceAPI::GetModelResource(GfxHND handle)
        {
            return m_privates->modelMgr->GetModel( static_cast<unsigned>( handle & 0xFFFF ) );
        }

        ufmt_loader::ModelInfo *ResourceAPI::GetModelInfo(const GfxHND &handle)
        {
            return m_privates->modelMgr->GetModelInfo( handle );
        }

        bool ResourceAPI::CheckAnimExistence(const std::string &animeName)
        {
            return m_privates->modelMgr->CheckAnimExistence(animeName);
        }

        GfxHND ResourceAPI::CreateAnimation(const ufmt_loader::AnimInfo &animeInfo)
        {
            return m_privates->modelMgr->CreateAnimation( animeInfo );
        }

        void ResourceAPI::DestroyAnimation(GfxHND &handle)
        {
            m_privates->modelMgr->DestroyAnimation( handle );
        }

        ufmt_loader::AnimInfo *ResourceAPI::GetAnimInfo(const GfxHND &handle)
        {
            return m_privates->modelMgr->GeAnimeInfo( handle );
        }

        void ResourceAPI::SetPrivates(void *priv, void *priv2)
        {
            m_privates->modelMgr = reinterpret_cast<ModelManager*>( priv );
            m_privates->textureMgr = reinterpret_cast<TextureManager*>( priv2 );
        }

        void ResourceAPI::Initialize(void)
        {
            m_privates = new privData;
        }

        void ResourceAPI::Uninitialize(void)
        {
            delete m_privates;
        }
    }
}
