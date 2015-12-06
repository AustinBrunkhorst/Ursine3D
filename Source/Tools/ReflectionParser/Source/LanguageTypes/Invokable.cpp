/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Invokable.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "LanguageTypes/Invokable.h"

#include <Utils.h>

Invokable::Invokable(const Cursor &cursor)
    : m_returnType( utils::GetQualifiedName( cursor.GetReturnType( ) ))
{
    auto type = cursor.GetType( );
    unsigned count = type.GetArgumentCount( );

    m_signature.clear( );

    for (unsigned i = 0; i < count; ++i)
    {
        auto argument = type.GetArgument( i );

        m_signature.emplace_back(
            utils::GetQualifiedName( argument )
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