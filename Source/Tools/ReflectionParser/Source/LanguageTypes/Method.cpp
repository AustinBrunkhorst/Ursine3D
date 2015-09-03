#include "Precompiled.h"

#include "LanguageTypes/Class.h"
#include "LanguageTypes/Method.h"

#include <Utils.h>

#include <boost/format.hpp>

Method::Method(const Cursor &cursor, const Namespace &currentNamespace, Class *parent)
    : LanguageType(cursor, currentNamespace)
    , Invokable(cursor)
    , m_isConst( cursor.IsConst( ) )
    , m_parent( parent )
    , m_name( cursor.GetSpelling( ) )
{
    
}

TemplateData Method::CompileTemplate(void) const
{
    TemplateData data = { TemplateData::Type::Object };

    data[ "name" ] = m_name;
        
    data[ "parentQualifiedName" ] = m_parent->m_qualifiedName;

    data[ "isAccessible" ] = utils::TemplateBool( isAccessible( ) );
    
    data[ "isVoidReturnType" ] = utils::TemplateBool( m_returnType == kReturnTypeVoid );

    data[ "qualifiedSignature" ] = getQualifiedSignature( );
    data[ "invocationBody" ] = getInvocationBody( );

    m_metaData.CompileTemplateData( data );

    return data;
}

bool Method::isAccessible(void) const
{
    return m_accessModifier == CX_CXXPublic && !m_metaData.GetFlag( kMetaDisable );
}

std::string Method::getQualifiedSignature(void) const
{
    std::string argsList;

    ursine::utils::Join( m_signature, ", ", argsList );

    std::string constNess = m_isConst ? " const" : "";

    return (boost::format( "%1%(%2%::*)(%3%)%4%" ) % m_returnType % m_parent->m_qualifiedName % argsList % constNess).str( );
}

std::string Method::getInvocationBody(void) const
{
    std::stringstream body;

    body << m_name << "( ";

    std::vector<std::string> calls;

    for (unsigned i = 0; i < m_signature.size( ); ++i)
        calls.emplace_back( (boost::format( "args[ %1% ].GetValue<%2%>( )" ) % i % m_signature[ i ]).str( ) );

    std::string joined;

    ursine::utils::Join( calls, ", ", joined );

    body << joined << " )";

    return body.str( );
}