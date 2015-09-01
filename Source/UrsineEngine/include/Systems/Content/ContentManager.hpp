/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ContentManager.hpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Utils.h"

namespace ursine
{
    template<typename AssetType>
    AssetType *ContentManager::Loader(const std::string &name, uint32 flags)
    {
        static_assert(false, "A content loader for this type does not exist.");

        return nullptr;
    }

    template<typename AssetType>
    AssetType *ContentManager::Copier(const AssetType *instance, uint32 flags)
    {
        // we don't want a compile time error, as some assets don't need a copier
        UError("Copier doesn't exist for this type.");

        return nullptr;
    }

    template<typename AssetType>
    AssetType *ContentManager::Load(const std::string &name, uint32 flags)
    {
        static_assert(std::is_base_of<Asset, AssetType>::value,
            "Type specified is not an asset!");

        std::string normalized = utils::NormalizePath(name);

        if (utils::IsFlagSet(flags, FORCE_LOAD))
        {
            goto hard_load;
        }
        else
        {
            auto asset = _cache.find(normalized);

            if (asset == _cache.end())
                goto hard_load;

            if (utils::IsFlagSet(flags, COPY))
            {
                auto copy = Copier<AssetType>(static_cast<AssetType*>(asset->second), flags);

                _copies.push_back(copy);

                return copy;
            }

            return static_cast<AssetType*>(asset->second);
        }

    hard_load:
        auto *load = Loader<AssetType>(normalized, flags);

        _cache.insert(std::make_pair(normalized, static_cast<Asset*>(load)));

        if (utils::IsFlagSet(flags, COPY))
        {
            load = Copier<AssetType>(load, flags);

            _copies.push_back(load);
        }

        return load;
    }
}