#include "Precompiled.h"

#include "LanguageTypes/Function.h"
#include "LanguageTypes/Class.h"

Function::Function(const Cursor &cursor, const Namespace &currentNamespace, Class *parent)
    : LanguageType( cursor, currentNamespace )
    , m_parent( parent )
{
        
}