#include "UrsinePrecompiled.h"

#include "MetaManager.h"

#include <algorithm>

namespace Ursine
{
    MetaManager::MetaManager(void) { }

    ////////////////////////////////////////////////////////////////////////////

    MetaManager::MetaManager(const std::initializer_list<Property> &properties)
    {
        for (auto &prop : properties)
            SetProperty( prop );
    }

    ////////////////////////////////////////////////////////////////////////////

    const MetaManager::Property *MetaManager::GetProperty(const std::string &key) const
    {
        auto search = m_properties.find( key );

        if (search == m_properties.end( ))
            return nullptr;

        return &search->second;
    }

    ////////////////////////////////////////////////////////////////////////////

    bool MetaManager::GetFlag(const std::string &key) const
    {
        return m_properties.find( key ) != m_properties.end( );
    }

    ////////////////////////////////////////////////////////////////////////////

    void MetaManager::SetProperty(const Property &prop)
    {
        m_properties[ prop.key ] = prop;
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    MetaManager::Property::Property(void) { }

    ////////////////////////////////////////////////////////////////////////////

    MetaManager::Property::Property(const std::string &key, const std::string &value)
        : key( key )
        , value( value ) { }

    ////////////////////////////////////////////////////////////////////////////

    int MetaManager::Property::ToInt(void) const
    {
        return stoi( value );
    }

    bool MetaManager::Property::ToBool(void) const
    {
        // 0 -> false
        // 1 -> true
        // "true" -> true (case insensitive)
        // "false" -> false (case insensitive)

        if (value == "0")
            return false;

        if (value == "1")
            return true;

        auto copy = value;

        // convert to lowercase
        transform( copy.begin( ), copy.end() , copy.begin( ), ::tolower );

        if (copy == "true")
            return true;

        return false;
    }

    ////////////////////////////////////////////////////////////////////////////
    
    float MetaManager::Property::ToFloat(void) const
    {
        return stof( value );
    }

    ////////////////////////////////////////////////////////////////////////////

    double MetaManager::Property::ToDouble(void) const
    {
        return stod( value );
    }
}
