#include "Precompiled.h"

#include "LanguageTypes/Invokable.h"

Invokable::Invokable(const Cursor &cursor)
    : m_returnType( cursor.GetReturnType( ).GetDisplayName( ) )
{
    auto type = cursor.GetType( );
    unsigned count = type.GetArgumentCount( );

    m_signature.clear( );

    for (unsigned i = 0; i < count; ++i)
    {
        auto argument = type.GetArgument( i );

        m_signature.emplace_back(
            argument.GetDisplayName( )
        );
    }
}

TemplateData Invokable::compileSignatureTemplate(void) const
{
    TemplateData data { TemplateData::Type::List };

    auto argCount = m_signature.size( );

    for (unsigned i = 0; i < argCount; ++i)
    {
        TemplateData argument { TemplateData::Type::Object };

        argument[ "type" ] = m_signature[ i ];
        argument[ "index" ] = std::to_string( i );
        argument[ "isLast" ] = utils::TemplateBool( i == argCount - 1 );

        data << argument;
    }

    return data;
}