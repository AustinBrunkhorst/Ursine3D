#include "UrsinePrecompiled.h"

#include "MetaManager.h"

#include <algorithm>

namespace ursine
{
    namespace meta
    {
        MetaManager::MetaManager(void) { }

        MetaManager::MetaManager(const MetaManager &rhs)
        {
            copy( rhs );
        }

        MetaManager::MetaManager(const MetaManager &&rhs)
            : m_properties( std::move( rhs.m_properties ) )
        {

        }

        const MetaManager &MetaManager::operator=(const MetaManager &rhs)
        {
            copy( rhs );

            return *this;
        }

        ///////////////////////////////////////////////////////////////////////

        MetaManager::MetaManager(const Initializer &properties)
        {
            for (auto &prop : properties)
            {
                UAssert( prop.first.IsValid( ), 
                    "Invalid meta property registered."
                    "Make sure the property type is enabled in reflection."
                );

                SetProperty( prop.first, prop.second );
            }
        }

        MetaManager::~MetaManager(void)
        {
            for (auto &prop : m_properties)
                delete prop.second;
        }

        ///////////////////////////////////////////////////////////////////////

        Variant MetaManager::GetProperty(Type type) const
        {
            auto search = m_properties.find( type );

            if (search == m_properties.end( ))
                return Variant { };

            return Variant { search->second, variant_policy::WrapObject( ) };
        }

        ///////////////////////////////////////////////////////////////////////

        void MetaManager::SetProperty(Type type, const MetaProperty *prop)
        {
            auto search = m_properties.find( type );

            // delete old property if it exists
            if (search != m_properties.end( ))
                delete search->second;

            m_properties[ type ] = prop;
        }

        MetaManager::PropertyList MetaManager::GetProperties(void) const
        {
            PropertyList properties;

            for (auto &property : m_properties)
                properties.emplace_back( property.second, variant_policy::WrapObject( ) );

            return properties;
        }

        Json MetaManager::SerializeJson(void) const
        {
            Json::object object { };

            for (auto &property : m_properties)
            {
                Variant instance { property.second, variant_policy::WrapObject( ) };

                auto type = instance.GetType( );

                object[ type.GetName( ) ] = type.SerializeJson( instance );
            }

            return object;
        }

        void MetaManager::copy(const MetaManager &rhs)
        {
            for (auto &prop : rhs.m_properties) 
            {
                m_properties[ prop.first ] = 
                    static_cast<MetaProperty*>( prop.second->Clone( ) );
            }
        }
    }
}
