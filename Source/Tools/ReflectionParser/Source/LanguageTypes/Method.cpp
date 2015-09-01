#include "Precompiled.h"

#include "LanguageTypes/Method.h"

Method::Method(const Cursor &cursor, const Namespace &currentNamespace)
    : LanguageType( cursor, currentNamespace )
    , Invokable( cursor )
    , m_isConst( cursor.GetType( ).IsConst( ) )
    , m_name( cursor.GetSpelling( ) )
{
    
}