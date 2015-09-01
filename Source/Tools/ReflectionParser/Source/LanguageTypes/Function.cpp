#include "Precompiled.h"

#include "LanguageTypes/Function.h"
#include "LanguageTypes/Class.h"

Function::Function(const Cursor &cursor, const Namespace &currentNamespace, Class *parent)
    : LanguageType( cursor, currentNamespace )
    , Invokable( cursor )
    , m_parent( parent )
    , m_name( cursor.GetSpelling( ) )
    , m_qualifiedName( utils::GetQualifiedName( cursor, currentNamespace ) )
{
        
}