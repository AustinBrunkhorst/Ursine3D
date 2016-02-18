#pragma once

#include "GUID.h"

namespace ursine
{
    namespace resources
    {
        class ResourceManager;

        class ResourceReference
        {
        public:
            template<typename ResourceType>
            ResourceType *Load(const ResourceReference &reference);

        private:
            friend class ResourceManager;

            ResourceManager *m_manager;
            GUID m_resourceGUID;

            ResourceReference(ResourceManager *manager, const GUID &resourceGUID);
        };
    }
}

#include "ResourceReference.hpp"