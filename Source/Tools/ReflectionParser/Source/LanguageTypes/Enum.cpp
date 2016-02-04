/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Enum.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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
    auto displayName = m_metaData.GetNativeString( native_property::DisplayName );

    if (displayName.empty( ))
        m_displayName = m_qualifiedName;
    else
        m_displayName = utils::GetQualifiedName( cursor, currentNamespace );

    std::cout << m_displayName << std::endl;

    // it's an anonymous enum?
    if (m_displayName.find( "anonymous enum" ) != std::string::npos)
        m_displayName = "";

    for (auto &child : cursor.GetChildren( ))
    {
        if (child.GetKind( ) == CXCursor_EnumConstantDecl)
        {
            MetaDataManager valueMeta( child );

            // don't add disabled values
            if (!valueMeta.GetFlag( native_property::Disable ))
                m_values.emplace_back( this, child );
        }
    }
}

bool Enum::ShouldCompile(void) const
{
    return isAccessible( );
}

TemplateData Enum::CompileTemplate(const ReflectionParser *context) const
{
    TemplateData data { TemplateData::Type::Object };

    data[ "displayName" ] = m_displayName;
    data[ "qualifiedName" ] = m_qualifiedName;
    data[ "ptrTypeEnabled" ] = utils::TemplateBool( m_ptrTypeEnabled );

    data[ "constPtrTypeEnabled" ] = 
        utils::TemplateBool( m_constPtrTypeEnabled );

    TemplateData members { TemplateData::Type::List };

    int i = 0;

    for (auto &value : m_values)
    {
        TemplateData member { TemplateData::Type::Object };

        member[ "key" ] = value.key;
        member[ "value" ] = value.value;
        member[ "isLast" ] = utils::TemplateBool( i == m_values.size( ) - 1 );

        members << member;

        ++i;
    }

    data[ "member" ] = members;

    m_metaData.CompileTemplateData( data, context );

    return data;
}

bool Enum::isAccessible(void) const
{
    return m_metaData.GetFlag( native_property::Enable );
}