#include "Precompiled.h"

#include "LanguageTypes/Constructor.h"

namespace language_types
{
    Constructor::Constructor(const Cursor &cursor, const Namespace &currentNamespace)
        : LanguageType( cursor, currentNamespace )
    {
        
    }
}