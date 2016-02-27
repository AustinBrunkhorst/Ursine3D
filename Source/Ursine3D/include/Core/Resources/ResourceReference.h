#pragma once

#include "ResourceReferenceEvent.h"

#include "GUID.h"

namespace ursine
{
    namespace resources
    {
        class ResourceManager;

        class ResourceReference
            : public meta::Object
            , public EventDispatcher<ResourceReferenceEventType, LambdaHandler>
        {
            META_OBJECT;

        public:
            Meta(Enable)
            ResourceReference(void);

            ResourceReference(const ResourceReference &rhs);
            ResourceReference &operator=(const ResourceReference &rhs);

            template<typename ResourceType>
            ResourceType *Load(bool ignoreCache = false) const;

            bool IsValid(void) const;

            const GUID &GetGUID(void) const;

            void OnSerialize(Json::object &output) const override;
            void OnDeserialize(const Json &input) override;

        private:
            friend class ResourceManager;
            friend class ecs::Component;

            ResourceManager *m_manager;
            GUID m_resourceGUID;

            ResourceReference(ResourceManager *manager, const GUID &resourceGUID);

            void setManager(ResourceManager *manager);
        } Meta(
            Enable, 
            WhiteListMethods,
            DisplayName( "ResourceReference" )
        );
    }
}

#include "ResourceReference.hpp"