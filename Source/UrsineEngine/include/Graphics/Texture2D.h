/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Texture2D.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "TextureConfig.h"

#include "ContentManager.h"

#include "JsonSerializer.h"

#include "Vec2.h"

namespace ursine
{
    class Texture2D 
        : public utils::NoCopy
        , public Asset
    {
        friend class ContentManager;

        Vec2 m_size;
        Vec2 m_realSize;

        TextureID m_id;

        uint64 m_uniqueID;

        TextureFilterMode m_filterMode;
        TextureWrapMode m_wrapMode;

    public:
        explicit Texture2D(const std::string &path);
        ~Texture2D(void);

        static Texture2D *FromFile(const std::string &filename);

        static void Bind(const Texture2D *texture);

        TextureID GetID(void) const;
        uint64 GetUniqueID(void) const;

        const Vec2 &GetSize(void) const;

        TextureFilterMode GetFilterMode(void) const;
        void SetFilterMode(TextureFilterMode mode);

        TextureWrapMode GetWrapMode(void) const;
        void SetWrapMode(TextureWrapMode mode);
    };

    extern template 
    Texture2D *ContentManager::Loader(const std::string &name, uint32 flags);

    extern template 
    Json JsonSerializer::Serialize(Texture2D* &instance);

    extern template 
    void JsonSerializer::Deserialize(const Json &data, Texture2D* &out);

    extern template
    Json JsonSerializer::Serialize(std::vector<Texture2D*> &instance);

    extern template
    void JsonSerializer::Deserialize(const Json &data, std::vector<Texture2D*> &out);
}