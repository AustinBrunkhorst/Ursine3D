#include "Precompiled.h"

#include "LanguageTypes/Class.h"
#include "LanguageTypes/Constructor.h"

#include <Utils.h>

Constructor::Constructor(
    const Cursor &cursor, 
    const Namespace &currentNamespace, 
    Class *parent
)
    : LanguageType( cursor, currentNamespace )
    , Invokable( cursor )
    , m_parent( parent )
{
        
}

bool Constructor::ShouldCompile(void) const
{
    return isAccessible( );
}

TemplateData Constructor::CompileTemplate(
    const ReflectionParser *context
) const
{
    TemplateData data { TemplateData::Type::Object };

    data[ "parentQualifiedName" ] = m_parent->m_qualifiedName;

    data[ "templateParameters" ] = getTemplateParameters( );

    data[ "invocationBody" ] = 
        context->LoadTemplatePartial( kPartialConstructorInvocation );

    data[ "dynamicInvocationBody" ] = 
        context->LoadTemplatePartial( kPartialDynamicConstructorInvocation );

    data[ "enableNonDynamic" ] = 
        utils::TemplateBool( 
            !m_metaData.GetFlag( native_property::DisableNonDynamicCtor ) 
        );

    data[ "dynamicWrapObject" ] = 
        utils::TemplateBool( 
            m_metaData.GetFlag( native_property::DynamicCtorWrap ) 
        );

    data[ "argument" ] = compileSignatureTemplate( );

    m_metaData.CompileTemplateData( data, context );

    return data;
}

bool Constructor::isAccessible(void) const
{
    if (m_accessModifier != CX_CXXPublic)
        return false;

    // if the parent wants white listed method, then we must have 
    // the enable flag
    if (m_parent->GetMetaData( ).GetFlag( native_property::WhiteListMethods ))
        return m_metaData.GetFlag( native_property::Enable );

    // must not be explicitly disabled
    return !m_metaData.GetFlag( native_property::Disable );
}

std::string Constructor::getTemplateParameters(void) const
{
    auto params( m_signature );

    params.insert( params.begin( ), m_parent->m_qualifiedName );

    std::string output;

    ursine::utils::Join( params, ", ", output );

    // parent type, arg types, ...
    return output;
}