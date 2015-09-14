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

#include "Vec2.h"

#include "Utils.h"

namespace ursine
{
    class Texture2D 
        : public utils::NoCopy
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
}