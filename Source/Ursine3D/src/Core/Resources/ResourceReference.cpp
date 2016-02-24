#include "UrsinePrecompiled.h"

#include "ResourceReference.h"

#include "ResourceManager.h"

namespace ursine
{
    namespace resources
    {
        ResourceReference::ResourceReference(void)
            : m_manager(nullptr) 
            , m_resourceGUID( GUIDNullGenerator( )( ) ) { }

        ResourceReference::ResourceReference(ResourceManager *manager, const GUID &resourceGUID)
            : m_manager( manager )
            , m_resourceGUID( resourceGUID ) { }

        bool ResourceReference::IsValid(void) const
        {
            return !!m_manager;
        }

        const GUID &ResourceReference::GetGUID(void) const
        {
            return m_resourceGUID;
        }

        void ResourceReference::OnSerialize(Json::object &output) const
        {
            
        }

        void ResourceReference::OnDeserialize(const Json &input)
        {
            
        }
    }
}