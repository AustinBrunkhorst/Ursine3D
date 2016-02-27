#pragma once

#include "ResourceData.h"
#include "ResourceReference.h"

#include "FileSystem.h"

#include "GUID.h"

namespace ursine
{
    namespace resources
    {
        class ResourceManager
        {
        public:
            ResourceManager(void);
            ~ResourceManager(void);

            // Directory where built resources exist
            void SetResourceDirectory(const fs::path &resourceDirectory);

            ResourceReference CreateReference(const GUID &guid);
            ResourceData::Handle LoadReference(const ResourceReference &reference, bool ignoreCache);

            bool ResourceExists(const GUID &guid) const;

        #if defined(URSINE_WITH_EDITOR)

            // Reloads an asset from a build if already exists in the cache
            void ReloadIfCached(const GUID &guid);

        #endif
        private:
            fs::path m_resourceDirectory;

            std::unordered_map<GUID, ResourceData::Handle, GUIDHasher> m_database;

            ResourceManager(const ResourceManager &rhs) = delete;
            ResourceManager &operator=(const ResourceManager &rhs) = delete;

            ResourceData::Handle hardLoadResource(const GUID &guid);
            ResourceData::Handle loadResource(const GUID &guid);

            fs::path getResourceFileName(const GUID &guid) const;
        };
    }
}

#include "ResourceManager.hpp"