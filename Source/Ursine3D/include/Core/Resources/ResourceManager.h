#pragma once

#include "ResourceData.h"
#include "ResourceReference.h"

#include "GUID.h"

namespace ursine
{
    namespace resources
    {
        class ResourceManager
        {
        public:
            template<typename ResourceType>
            ResourceType *LoadReference(const ResourceReference &reference);

        private:
            std::unordered_map<std::string, ResourceData *> m_nameReference;
            std::unordered_map<GUID, ResourceData *, GUIDHasher> m_database;
        };
    }
}

#include "ResourceManager.hpp"