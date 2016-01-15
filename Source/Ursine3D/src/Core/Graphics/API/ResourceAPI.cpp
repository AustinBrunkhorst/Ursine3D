/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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

        //get model handle
        GfxHND ResourceAPI::GetModelHandle(const char *name)
        {
            return m_privates->modelMgr->GetModelIDByName(name);
        }

        //get texture handle
        GfxHND ResourceAPI::GetTexHandle(const char *name)
        {
            return m_privates->textureMgr->GetTextureIDByName(name);
        }

        GfxHND ResourceAPI::CreateTexture(const unsigned width, const unsigned height)
        {
            return m_privates->textureMgr->CreateDynamicTexture(width, height);
        }

        void ResourceAPI::ResizeTexture(GfxHND& handle, const unsigned width, const unsigned height)
        {
            m_privates->textureMgr->ResizeDynamicTexture(handle, width, height);
        }

        void ResourceAPI::DestroyTexture(GfxHND& handle)
        {
            m_privates->textureMgr->DestroyDynamicTexture(handle);
        }

        ModelResource *ResourceAPI::GetModel(const GfxHND & handle)
        {
            return m_privates->modelMgr->GetModel(handle);
        }

        ModelResource * ResourceAPI::GetModel(const std::string & modelName)
        {
            return m_privates->modelMgr->GetModel(modelName);
        }

        void ResourceAPI::SetPrivates(void *priv, void *priv2)
        {
            m_privates->modelMgr = reinterpret_cast<ModelManager*>(priv);
            m_privates->textureMgr = reinterpret_cast<TextureManager*>(priv2);
        }

        void ResourceAPI::Initialize()
        {
            m_privates = new privData;
        }

        void ResourceAPI::Uninitialize()
        {
            delete m_privates;
        }
    }

}