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
            EditorField(
                std::string guid,
                GetGUIDString,
                SetGUIDString
            );

            Meta(Enable)
            ResourceReference(void);
            ResourceReference(const GUID &resourceGUID);
            ResourceReference(const ResourceReference &rhs);

            ResourceReference &operator=(const ResourceReference &rhs);

            bool operator==(const ResourceReference &rhs);

            template<typename ResourceType>
            ResourceType *Load(ResourceManager &resourceManager, bool ignoreCache = false) const;

            const GUID &GetGUID(void) const;

            std::string GetGUIDString(void) const;
            void SetGUIDString(const std::string &guid);

        private:
            GUID m_resourceGUID;

        } Meta(
            Enable, 
            WhiteListMethods,
            DisplayName( "ResourceReference" )
        );
    }
}

#include "ResourceReference.hpp"