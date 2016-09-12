/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ResourcePipelineProperties.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ResourcePipelineProperties.h"

ResourceImporterConfig::ResourceImporterConfig(const std::string &extension, const ursine::meta::Type &defaultProcessor)
    : fileExtensions( ursine::Array<std::string> { extension } )
    , defaultProcessor( defaultProcessor )
{
    auto &extensions = const_cast<ursine::Array<std::string>&>( fileExtensions );

    ursine::utils::MakeLowerCase( extensions[ 0 ] );
}

ResourceImporterConfig::ResourceImporterConfig(const ExtensionList &extensions, const ursine::meta::Type &defaultProcessor)
    : fileExtensions( extensions )
    , defaultProcessor( defaultProcessor )
{
    for (auto &extension : const_cast<ursine::Array<std::string>&>( fileExtensions ))
        ursine::utils::MakeLowerCase( extension );
}

///////////////////////////////////////////////////////////////////////////////

ResourceProcessorConfig::ResourceProcessorConfig(const ursine::meta::Type &optionsType)
    : optionsType( optionsType ) { }

///////////////////////////////////////////////////////////////////////////////

ResourceSyncConfig::ResourceSyncConfig(const FileExclusionList &exclusionExpressions)
    : exclusionExpressions( exclusionExpressions )
{
    for (auto &expression : exclusionExpressions)
    {
        try
        {
            m_exclusionExpressions.emplace_back( expression );
        }
        catch (boost::regex_error &e)
        {
            UError( "Invalid sync exclusion expression.\nerror: %s", e.what( ) );
        }
    }
}

const std::vector<boost::regex> &ResourceSyncConfig::GetBuiltExclusionExpressions(void) const
{
    return m_exclusionExpressions;
}

bool ResourceSyncConfig::IsExcluded(const std::string &path) const
{
    boost::cmatch match;

    auto *str = path.c_str( );

    for (auto &expression : m_exclusionExpressions)
    {
        if (boost::regex_search( str, match, expression ))
            return true;
    }

    return false;
}