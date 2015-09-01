#include "Precompiled.h"

#include "LanguageTypes/Global.h"
#include "LanguageTypes/Class.h"

Global::Global(const Cursor &cursor, const Namespace &currentNamespace, Class *parent)
    : LanguageType( cursor, currentNamespace )
    , m_isConst( cursor.GetType( ).IsConst( ) )
    , m_parent( parent )
    , m_name( cursor.GetSpelling( ) )
    , m_qualifiedName( utils::GetQualifiedName( cursor, currentNamespace ) )
{
    auto displayName = m_metaData.GetProperty( kMetaDisplayName );

    if (displayName.empty( ))
    {
        m_displayName = m_qualifiedName;
    }
    else
    {
        m_displayName = utils::GetQualifiedName( displayName, currentNamespace );
    }
}