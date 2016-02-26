#pragma once

#include "GUID.h"

namespace ursine
{
    namespace resources
    {
        class ResourceManager;

        class ResourceReference : public meta::Object
        {
            META_OBJECT;

        public:
            Meta(Enable)
            ResourceReference(void);

            template<typename ResourceType>
            ResourceType *Load(bool ignoreCache = false) const;

            bool IsValid(void) const;

            const GUID &GetGUID(void) const;

        private:
            friend class ResourceManager;
            friend class Component;

            mutable ResourceManager *m_manager;
            GUID m_resourceGUID;

            ResourceReference(ResourceManager *manager, const GUID &resourceGUID);

            void OnSerialize(Json::object &output) const override;
            void OnDeserialize(const Json &input) override;
        } Meta(Enable, WhiteListMethods, DisplayName( "ResourceReference" ));
    }
}

#include "ResourceReference.hpp"