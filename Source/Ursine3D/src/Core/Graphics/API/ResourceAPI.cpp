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