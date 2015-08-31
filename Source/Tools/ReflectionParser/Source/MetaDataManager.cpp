#include "Precompiled.h"

#include "MetaDataManager.h"

MetaDataManager::MetaDataManager(const Cursor &cursor)
{
    URSINE_TODO( "Iterate though CX_Cursor_AnnotateAttr and extract properties" );
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