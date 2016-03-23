#include "UrsinePrecompiled.h"

#include "ResourceReference.h"

namespace ursine
{
    namespace resources
    {
        ResourceReference::ResourceReference(void)
            : m_resourceGUID( GUIDNullGenerator( )( ) )
        {

        }

        ResourceReference::ResourceReference(const GUID &guid)
            : m_resourceGUID( guid )
        {

        }

        ResourceReference::ResourceReference(const ResourceReference &rhs)
            : m_resourceGUID( rhs.m_resourceGUID )
        {

        }

        ResourceReference &ResourceReference::operator=(const ResourceReference &rhs)
        {
            m_resourceGUID = rhs.m_resourceGUID;

            return *this;
        }

        bool ResourceReference::operator==(const ResourceReference &rhs)
        {
            return m_resourceGUID == rhs.GetGUID( );
        }

        const GUID &ResourceReference::GetGUID(void) const
        {
            return m_resourceGUID;
        }

        std::string ResourceReference::GetGUIDString(void) const
        {
            return to_string( m_resourceGUID );
        }

        void ResourceReference::SetGUIDString(const std::string &guid)
        {
            try
            {
                m_resourceGUID = GUIDStringGenerator( )( guid );
            }
            // parse error
            catch (...)
            {
                m_resourceGUID = GUIDNullGenerator( )( );
            }
        }
    }
}