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
#include "FontManager.h"

namespace ursine
{
    namespace graphics
    {
        struct ResourceAPI::privData
        {
            ModelManager *modelMgr;
            TextureManager *textureMgr;
            FontManager *fontMgr;
        };

        ResourceAPI::ResourceAPI(void)
            : m_privates(nullptr) { }

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

        GfxHND ResourceAPI::CreateTexture(const uint8_t *binaryData, size_t binarySize, unsigned width, unsigned height)
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

        void ResourceAPI::LoadFontTextures(GfxHND handle)
        {
            m_privates->textureMgr->LoadFontTextures(handle, m_privates->fontMgr);
        }

        void ResourceAPI::UnloadFontTextures(GfxHND handle)
        {
            m_privates->textureMgr->UnloadFontTextures(handle, m_privates->fontMgr);
        }

        void ResourceAPI::GetBinaryInformation(GfxHND handle, uint8_t **dataPtr, size_t &binarySize)
        {
            m_privates->textureMgr->GetBinaryInformation( handle, dataPtr, binarySize );
        }

        /////////////////////////////////////////////////////////
        // MODEL
        /////////////////////////////////////////////////////////

        GfxHND ResourceAPI::CreateModel(const resources::UModelData::Handle &model)
        {
            return m_privates->modelMgr->CreateModel( model );
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

        void ResourceAPI::PrepForFont(void)
        {
            m_privates->fontMgr->LockFontManager();
        }

        GfxHND ResourceAPI::CreateBitmapFont(const uint8_t *binaryData, size_t binarySize)
        {
            return m_privates->fontMgr->CreateBitmapFont( binaryData, binarySize );
        }

        void ResourceAPI::DestroyBitmapFont(GfxHND &fontHandle)
        {
            m_privates->fontMgr->DestroyBitmapFont( fontHandle );
        }

        void ResourceAPI::RegisterTexture(GfxHND fontHandle, const std::string &fontName, GfxHND textureHandle)
        {
            m_privates->fontMgr->RegisterTexture( fontHandle, fontName, textureHandle );
        }

        void ResourceAPI::FinishFont(void)
        {
            m_privates->fontMgr->UnlockFontManager( );
        }

        void ResourceAPI::SetPrivates(void *priv, void *priv2, void *priv3)
        {
            m_privates->modelMgr = reinterpret_cast<ModelManager*>( priv );
            m_privates->textureMgr = reinterpret_cast<TextureManager*>( priv2 );
            m_privates->fontMgr = reinterpret_cast<FontManager*>(priv3);
        }

        void ResourceAPI::Initialize(void)
        {
            if (!m_privates)
                m_privates = new privData;
        }

        void ResourceAPI::Uninitialize(void)
        {
            delete m_privates;
        }
    }
}
