/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ContentManager.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "System.h"
#include "Asset.h"
#include "UrsineTypes.h"

#include <unordered_map>
#include <memory>

namespace ursine
{
    class ContentManager : public System
    {
        // cached assets
        std::unordered_map<std::string, Asset*> _cache;

        // copied assets
        std::vector<Asset*> _copies;

        template<typename AssetType>
        AssetType *Loader(const std::string &name, uint32 flags);

        template<typename AssetType>
        AssetType *Copier(const AssetType *instance, uint32 flags);

    public:
        static const uint8 FORCE_LOAD = 1;
        static const uint8 COPY = 2;

        ContentManager(void);
        ~ContentManager(void);

        template<typename AssetType>
        AssetType *Load(const std::string &name, uint32 flags = 0);

        std::shared_ptr<std::string> LoadText(const std::string &name, uint32 flags = 0);

        void Unload(const std::string &name);
    };

    extern ContentManager *gContentManager;
}

#include "ContentManager.hpp"
