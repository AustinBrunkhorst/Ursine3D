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
            ResourceType *Load(bool ignoreCache = false) const;

            const GUID &GetGUID(void) const;

        private:
            friend class ResourceManager;

            mutable ResourceManager *m_manager;
            GUID m_resourceGUID;

            ResourceReference(ResourceManager *manager, const GUID &resourceGUID);
        };
    }
}

#include "ResourceReference.hpp"