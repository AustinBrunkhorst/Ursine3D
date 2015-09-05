#include "UrsinePrecompiled.h"

#include "MetaManager.h"

#include <algorithm>

namespace ursine
{
    namespace meta
    {
        MetaManager::MetaManager(void) { }

        ////////////////////////////////////////////////////////////////////////////

        MetaManager::MetaManager(const Initializer &properties)
        {
            for (auto &prop : properties)
            {
                UAssert( prop.first.IsValid( ), 
                    "Invalid meta property registered. Make sure the property type is enabled in reflection."
                );

                SetProperty( prop.first, prop.second );
            }
        }

        MetaManager::~MetaManager(void)
        {
            URSINE_TODO( "manage leaks" );
            /*for (auto &prop : m_properties)
                delete prop.second;*/
        }

        ////////////////////////////////////////////////////////////////////////////

        Variant MetaManager::GetProperty(Type type) const
        {
            auto search = m_properties.find( type );

            if (search == m_properties.end( ))
                return Variant { };

            return Variant { *search->second };
        }

        ////////////////////////////////////////////////////////////////////////////

        void MetaManager::SetProperty(Type type, const MetaProperty *prop)
        {
            auto search = m_properties.find( type );

            // delete old property if it exists
            if (search != m_properties.end( ))
                delete search->second;

            m_properties[ type ] = prop;
        }
    }
}
