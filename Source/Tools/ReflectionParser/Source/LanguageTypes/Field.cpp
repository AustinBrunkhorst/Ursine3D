#include "Precompiled.h"

#include "LanguageTypes/Field.h"

Field::Field(const Cursor &cursor, const Namespace &currentNamespace)
    : LanguageType( cursor, currentNamespace )
    , m_name( cursor.GetSpelling( ) )
    , m_type( cursor.GetType( ).GetDisplayName( ) )
{
    auto displayName = m_metaData.GetProperty( kMetaDisplayName );

    if (displayName.empty( ))
        m_displayName = m_name;
    else
        m_displayName = displayName;
}