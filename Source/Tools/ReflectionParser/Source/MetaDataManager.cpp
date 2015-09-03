#include "Precompiled.h"

#include "MetaDataManager.h"

#include <Utils.h>

#include <boost/regex.hpp>
#include <boost/algorithm/string/replace.hpp>

const TemplateData::PartialType MetaDataManager::m_tmplInitializerListPartial = [](void)
{
    return 
        "{{#metaProperty}}"
        "{ \"{{& key}}\"{{^ isFlag}}, \"{{& value}}\"{{/isFlag}} }{{^ isLast}}, {{/isLast}}"
        "{{/metaProperty}}";
};

MetaDataManager::MetaDataManager(const Cursor &cursor)
{
    for (auto &child : cursor.GetChildren( ))
    {
        if (child.GetKind( ) != CXCursor_AnnotateAttr)
            continue;

        for (auto &prop : extractProperties( child ))
            m_properties[ prop.first ] = prop.second;
    }
}

std::string MetaDataManager::GetProperty(const std::string &key) const
{
    auto search = m_properties.find( key );

    // use an empty string by default
    return search == m_properties.end( ) ? "" : search->second;
}

bool MetaDataManager::GetFlag(const std::string &key) const
{
    return m_properties.find( key ) == m_properties.end( ) ? false : true;
}

void MetaDataManager::CompileTemplateData(TemplateData &data) const
{
    TemplateData propertyData { TemplateData::Type::List };

    std::vector<std::string> lines;

    // list of keywords to ignore in the initializer list
    static const std::vector<std::string> reservedKeywords
    {
        kMetaEnable,
        kMetaDisable,
        kMetaDisablePtrType,
        kMetaDisableConstPtrType
    };

    int i = 0;

    auto propertyCount = m_properties.size( ) - 1;

    for (auto &prop : m_properties)
    {
        TemplateData item { TemplateData::Type::Object };

        // skip reserved keywords
        if (std::find( reservedKeywords.begin( ), reservedKeywords.end( ), prop.first ) != reservedKeywords.end( ))
        {
            --propertyCount;

            continue;
        }

        auto key = prop.first;
        auto value = prop.second;

        boost::algorithm::replace_all( key, "\"", "\\\"" );
        boost::algorithm::replace_all( value, "\"", "\\\"" );

        item[ "key" ] = key;
        item[ "value" ] = value;
        
        item[ "isFlag" ] = utils::TemplateBool( value.empty( ) );
        item[ "isLast" ] = utils::TemplateBool( i == propertyCount );

        propertyData << item;

        ++i;
    }

    data[ "metaProperty" ] = propertyData;
    data[ "metaDataInitializerList" ] = m_tmplInitializerListPartial;
}

std::vector<MetaDataManager::Property> MetaDataManager::extractProperties(const Cursor &cursor) const
{
    std::vector<Property> properties;
    std::vector<std::string> subProperties;

    // split by "," that are not contained in double quotes
    static const boost::regex delimiter( ",(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)" );

    auto meta = cursor.GetDisplayName( );
    
    ursine::utils::Split( meta, delimiter, subProperties );

    // split by "=" that are not contained in double quotes
    static const boost::regex pairDelimiter( "=(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)" );

    for (auto &prop : subProperties)
    {
        std::vector<std::string> pair;

        ursine::utils::Split( prop, pairDelimiter, pair );

        // property key
        auto key = trimPropertyName( pair[ 0 ] );

        boost::algorithm::replace_all( key, "`", "\"" );

        decltype(pair) values( pair.begin( ) + 1, pair.end( ) );

        // rest of the string in the pair
        std::string value;

        ursine::utils::Join( values, "=", value );

        value = trimPropertyName( value );

        boost::algorithm::replace_all( value, "`", "\"" );

        properties.emplace_back( key, value );
    }

    return properties;
}

std::string MetaDataManager::trimPropertyName(const std::string &input) const
{
    auto output( input );

    static const boost::regex whitespace( "\\s(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)" );

    output = boost::regex_replace( output, whitespace, "", boost::match_default | boost::format_all );

    boost::algorithm::replace_all( output, "\"", "" );

    return output;
}