#include "Precompiled.h"

#include "LanguageTypes/Function.h"
#include "LanguageTypes/Class.h"

namespace language_types
{
    Function::Function(const Cursor &cursor, const Namespace &currentNamespace, Class *parent)
        : LanguageType( cursor, currentNamespace )
        , m_parent( parent )
    {
        
    }
}