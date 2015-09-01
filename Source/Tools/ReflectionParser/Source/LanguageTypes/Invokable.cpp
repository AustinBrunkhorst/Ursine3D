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