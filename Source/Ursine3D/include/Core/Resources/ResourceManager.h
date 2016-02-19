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
            ResourceData::Handle LoadReference(const ResourceReference &reference);

        private:
            fs::path m_resourceDirectory;

            std::unordered_map<GUID, ResourceData::Handle, GUIDHasher> m_database;

            ResourceManager(const ResourceManager &rhs) = delete;
            ResourceManager &operator=(const ResourceManager &rhs) = delete;

            ResourceData::Handle loadResource(const GUID &guid);

            fs::path getResourceFileName(const GUID &guid);
        };
    }
}

#include "ResourceManager.hpp"