#include "Precompiled.h"

#include "LanguageTypes/Function.h"
#include "LanguageTypes/Class.h"

#include <Utils.h>

#include <boost/format.hpp>

Function::Function(const Cursor &cursor, const Namespace &currentNamespace, Class *parent)
    : LanguageType( cursor, currentNamespace )
    , Invokable( cursor )
    , m_parent( parent )
    , m_name( cursor.GetSpelling( ) )
    , m_qualifiedName( utils::GetQualifiedName( cursor, currentNamespace ) )
{
        
}

TemplateData Function::CompileTemplate(void) const
{
    TemplateData data = { TemplateData::Type::Object };

    data[ "name" ] = m_name;
    data[ "qualifiedName" ] = m_qualifiedName;

    if (m_parent)
        data[ "parentQualifiedName" ] = m_parent->m_qualifiedName;

    data[ "isAccessible" ] = utils::TemplateBool( isAccessible( ) );
    
    data[ "isVoidReturnType" ] = utils::TemplateBool( m_returnType == kReturnTypeVoid );

    data[ "qualifiedSignature" ] = getQualifiedSignature( );
    data[ "invocationBody" ] = getInvocationBody( );

    m_metaData.CompileTemplateData( data );

    return data;
}

bool Function::isAccessible(void) const
{
    if (m_parent && m_accessModifier != CX_CXXPublic)
        return false;

    return m_metaData.GetFlag( kMetaEnable );
}

std::string Function::getQualifiedSignature(void) const
{
    std::string argsList;

    ursine::utils::Join( m_signature, ", ", argsList );

    return (boost::format( "%1%(*)(%2%)" ) % m_returnType % argsList).str( );
}

std::string Function::getInvocationBody(void) const
{
    std::stringstream body;

    body << m_qualifiedName << "( ";

    std::vector<std::string> calls;

    for (unsigned i = 0; i < m_signature.size( ); ++i)
        calls.emplace_back( (boost::format( "args[ %1% ].GetValue<%2%>( )" ) % i % m_signature[ i ]).str( ) );

    std::string joined;

    ursine::utils::Join( calls, ", ", joined );

    body << joined << " )";

    return body.str( );
}