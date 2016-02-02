#pragma once

#include "CoreSystem.h"

#include "ResourceConfig.h"

#include "ResourceHandle.h"

namespace ursine
{
    class Resource;

    class ResourceManager : public core::CoreSystem
    {
        CORE_SYSTEM;

    public:
        void SetAssetDirectory(const fs::path &assetDirectory);
        void SetLibraryDirectory(const fs::path &libraryDirectory);

    private:
        friend class Resource;

        fs::path m_assetDirectory;
        fs::path m_libraryDirectory;

        std::unordered_map<std::string, Resource*> m_nameToResource;
        std::unordered_map<GUID, Resource*, GUIDHasher> m_guidToResource;
    };
}