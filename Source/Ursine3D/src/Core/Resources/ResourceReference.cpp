#include "UrsinePrecompiled.h"

#include "ResourceReference.h"

#include "ResourceManager.h"

namespace ursine
{
    namespace resources
    {
        ResourceReference::ResourceReference(void)
            : EventDispatcher( this )
            , m_manager( nullptr ) 
            , m_resourceGUID( GUIDNullGenerator( )( ) ) { }

        ResourceReference::ResourceReference(const ResourceReference &rhs)
            : EventDispatcher( this )
            , m_manager( rhs.m_manager ) 
            , m_resourceGUID( rhs.m_resourceGUID )
        {
            // TODO: test this
        }

        ResourceReference::ResourceReference(ResourceManager *manager, const GUID &resourceGUID)
            : EventDispatcher( this )
            , m_manager( manager )
            , m_resourceGUID( resourceGUID ) { }

        ResourceReference &ResourceReference::operator=(const ResourceReference &rhs)
        {
            if (rhs.m_resourceGUID != m_resourceGUID)
            {
                m_resourceGUID = rhs.m_resourceGUID;

                Dispatch( RR_REFERENCED_RESOURCE_CHANGED, EventArgs::Empty );
            }

            return *this;
        }

        bool ResourceReference::IsValid(void) const
        {
            return m_manager != nullptr;
        }

        const GUID &ResourceReference::GetGUID(void) const
        {
            return m_resourceGUID;
        }

        void ResourceReference::OnSerialize(Json::object &output) const
        {
            output[ "guid" ] = to_string( m_resourceGUID );
        }

        void ResourceReference::OnDeserialize(const Json &input)
        {
            m_resourceGUID = GUIDStringGenerator( )( input[ "guid" ].string_value( ) );
        }
    }
}