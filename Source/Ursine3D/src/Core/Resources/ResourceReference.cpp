/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ResourceReference.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ResourceReference.h"

namespace ursine
{
    namespace resources
    {
        ResourceReference::ResourceReference(void)
            : m_resourceGUID( kNullGUID )
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
                m_resourceGUID = kNullGUID;
            }
        }
    }
}