#include "Precompiled.h"

#include "LanguageTypes/Method.h"

namespace language_types
{
    Method::Method(const Cursor &cursor, const Namespace &currentNamespace)
        : LanguageType( cursor, currentNamespace )
    {
        
    }
}