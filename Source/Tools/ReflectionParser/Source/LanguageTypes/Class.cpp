#include "Precompiled.h"

#include "LanguageTypes/Class.h"

BaseClass::BaseClass(const Cursor &cursor)
    : name( cursor.GetType( ).GetDisplayName( ) )
{

}

Class::Class(const Cursor &cursor, const Namespace &currentNamespace)
    : LanguageType( cursor, currentNamespace )
    , m_name( cursor.GetDisplayName( ) )
    , m_qualifiedName( cursor.GetType( ).GetDisplayName( ) )
{
    auto displayName = m_metaData.GetProperty( kMetaDisplayName );

    if (displayName.empty( ))
        m_displayName = m_qualifiedName;
    else
        m_displayName = utils::GetQualifiedName( cursor, currentNamespace );

    for (auto &child : cursor.GetChildren( ))
    {
        switch (child.GetKind( ))
        {
        case CXCursor_CXXBaseSpecifier:
            m_baseClasses.emplace_back( child );
            break;
        // constructor
        case CXCursor_Constructor:
            m_constructors.emplace_back( child, currentNamespace );
            break;
        // field
        case CXCursor_FieldDecl:
            m_fields.emplace_back( child, currentNamespace );
            break;
        // static field
        case CXCursor_VarDecl:
            m_staticFields.emplace_back( child, Namespace( ), this );
            break;
        // method / static method
        case CXCursor_CXXMethod:
            if (child.IsStatic( )) 
                m_staticMethods.emplace_back( child, Namespace( ), this );
            else 
                m_methods.emplace_back( child, currentNamespace );
            break;
        default:
            break;
        }
    }
}

Class::~Class(void)
{

}