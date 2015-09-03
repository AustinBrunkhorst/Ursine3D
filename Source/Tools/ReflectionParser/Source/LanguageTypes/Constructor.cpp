#include "Precompiled.h"

#include "LanguageTypes/Class.h"
#include "LanguageTypes/Constructor.h"

#include <Utils.h>

#include <boost/format.hpp>

Constructor::Constructor(const Cursor &cursor, const Namespace &currentNamespace, Class *parent)
    : LanguageType( cursor, currentNamespace )
    , Invokable( cursor )
    , m_parent( parent )
{
        
}

TemplateData Constructor::CompileTemplate(void) const
{
    TemplateData data { TemplateData::Type::Object };

    data[ "isAccessible" ] = utils::TemplateBool( isAccessible( ) );
    data[ "templateParameters" ] = getTemplateParameters( );
    data[ "invocationBody" ] = getInvocationBody( );

    m_metaData.CompileTemplateData( data );

    return data;
}

bool Constructor::isAccessible(void) const
{
    return m_accessModifier == CX_CXXPublic && !m_metaData.GetFlag( kMetaDisable );
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

std::string Constructor::getInvocationBody(void) const
{
    std::stringstream body;

    body << m_parent->m_qualifiedName << "(";

    std::vector<std::string> calls;

    for (unsigned i = 0; i < m_signature.size( ); ++i)
        calls.emplace_back( (boost::format( "args[ %1% ].GetValue<%2%>( )" ) % i % m_signature[ i ]).str( ) );

    std::string joined;

    ursine::utils::Join( calls, ",", joined );

    body << joined << " )";

    return body.str( );
}