#include "Precompiled.h"

#include "LanguageTypes/Enum.h"

Enum::Value::Value(Enum *parent, const Cursor &cursor)
    : key( cursor.GetDisplayName( ) )
    , value( parent->m_qualifiedName + "::" + key )
{

}

Enum::Enum(const Cursor &cursor, const Namespace &currentNamespace)
    : LanguageType( cursor, currentNamespace )
    , m_name( cursor.GetType( ).GetDisplayName( ) )
    , m_qualifiedName( m_name )
{
    auto displayName = m_metaData.GetProperty( kMetaDisplayName );

    if (displayName.empty( ))
        m_displayName = m_qualifiedName;
    else
        m_displayName = utils::GetQualifiedName( cursor, currentNamespace );

    // it's an anonymous enum?
    if (m_displayName.find( "anonymous enum" ) != std::string::npos)
        m_displayName = "";

    for (auto &child : cursor.GetChildren( ))
    {
        if (child.GetKind( ) == CXCursor_EnumConstantDecl)
            m_values.emplace_back( this, child );
    }
}

void Enum::LoadAnonymous(std::vector<Global*> &output, const Cursor &cursor, const Namespace &currentNamespace)
{
    URSINE_TODO( "..." );
}