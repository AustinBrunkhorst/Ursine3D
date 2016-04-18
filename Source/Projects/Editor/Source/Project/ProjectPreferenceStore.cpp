/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ProjectPreferenceStore.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ProjectPreferenceStore.h"

#include "Project.h"

namespace
{
    const auto kCategoryExtension = "prefs";
}

ProjectPreferenceStore::ProjectPreferenceStore(Project *project)
    : m_project( project )
{
    
}

const ursine::Json &ProjectPreferenceStore::Load(
    const std::string &category, 
    const std::string &item, 
    const ursine::Json &defaultValue
)
{
    checkCategoryLoaded( category );

    return m_store[ category ].try_emplace( item, defaultValue ).first->second;
}

void ProjectPreferenceStore::Store(
    const std::string &category, 
    const std::string &item, 
    const ursine::Json &value
)
{
    checkCategoryLoaded( category );

    m_store[ category ][ item ] = value;

    storeCategory( category );
}

void ProjectPreferenceStore::checkCategoryLoaded(const std::string &category)
{
    if (!m_loadedCategories.count( category ))
    {
        loadCategory( category );

        m_loadedCategories.insert( category );
    }
}

void ProjectPreferenceStore::loadCategory(const std::string &category)
{
    std::string jsonText;
    auto path = getCategoryPath( category );

    if (ursine::fs::LoadAllText( path.string( ), jsonText ))
    {
        std::string error;
        auto json = ursine::Json::parse( jsonText, error );

        if (json.is_object( ))
            m_store[ category ] = json.object_items( );
    }
}

void ProjectPreferenceStore::storeCategory(const std::string &category)
{
    auto jsonText = ursine::Json( m_store[ category ] ).dump( true );
    auto path = getCategoryPath( category );

    ursine::fs::WriteAllText( path.string( ), jsonText );
}

ursine::fs::path ProjectPreferenceStore::getCategoryPath(const std::string &category)
{
    return (m_project->GetConfig( ).preferencesDirectory / category)
        .replace_extension( kCategoryExtension );
}