#include "Precompiled.h"

#include "LanguageTypes/Class.h"
#include "LanguageTypes/Field.h"

const TemplateData::PartialType Field::m_getterPartial = [](void)
{
    return
        "{{#hasExplicitGetter}}"
            "{{& explicitGetter}}( )"
        "{{/hasExplicitGetter}}"
        "{{^hasExplicitGetter}}"
            "instance.{{name}}"
        "{{/hasExplicitGetter}}";
};

const TemplateData::PartialType Field::m_setterPartial = [](void)
{
    return
        "{{#hasExplicitSetter}}"
            "instance.{{& explicitSetter}}( value.GetValue<{{type}}>( ) );"
        "{{/hasExplicitSetter}}"
        "{{^hasExplicitSetter}}"
            "instance.{{name}} = value.GetValue<{{type}}>( );"
        "{{/hasExplicitSetter}}";
};

Field::Field(const Cursor &cursor, const Namespace &currentNamespace, Class *parent)
    : LanguageType( cursor, currentNamespace )
    , m_isConst( cursor.GetType( ).IsConst( ) )
    , m_hasExplicitGetter( m_metaData.GetFlag( kMetaExplicitGetter ) )
    , m_hasExplicitSetter( m_metaData.GetFlag( kMetaExplicitSetter ) )
    , m_parent( parent )
    , m_name( cursor.GetSpelling( ) )
    , m_type( cursor.GetType( ).GetDisplayName( ) )
{
    auto displayName = m_metaData.GetProperty( kMetaDisplayName );

    if (displayName.empty( ))
        m_displayName = m_name;
    else
        m_displayName = displayName;
}

TemplateData Field::CompileTemplate(void) const
{
    TemplateData data = { TemplateData::Type::Object };

    data[ "name" ] = m_name;
    data[ "displayName" ] = m_displayName;
    data[ "type" ] = m_type;

    data[ "isAccessible" ] = utils::TemplateBool( isAccessible( ) );
    data[ "hasParent" ] = utils::TemplateBool( !!m_parent );

    data[ "parentQualifiedName" ] = m_parent->m_qualifiedName;

    // getter

    data[ "isGetterAccessible" ] = utils::TemplateBool( isGetterAccessible( ) );
    data[ "hasExplicitGetter" ] = utils::TemplateBool( m_hasExplicitGetter );
    data[ "explicitGetter" ] = m_metaData.GetProperty( kMetaExplicitGetter );
    data[ "getterBody" ] = m_getterPartial;

    // setter

    data[ "isSetterAccessible" ] = utils::TemplateBool( isSetterAccessible( ) );
    data[ "hasExplicitSetter" ] = utils::TemplateBool( m_hasExplicitSetter );
    data[ "explicitSetter" ] = m_metaData.GetProperty( kMetaExplicitSetter );
    data[ "setterBody" ] = m_setterPartial;

    m_metaData.CompileTemplateData( data );

    return data;
}

bool Field::isAccessible(void) const
{
    return (m_hasExplicitGetter || m_hasExplicitSetter) ||
        (m_accessModifier == CX_CXXPublic && !m_metaData.GetFlag( kMetaDisable ));
}

bool Field::isGetterAccessible(void) const
{
    return m_hasExplicitGetter || m_accessModifier == CX_CXXPublic;
}

bool Field::isSetterAccessible(void) const
{
    return m_hasExplicitSetter || m_accessModifier == CX_CXXPublic;
}