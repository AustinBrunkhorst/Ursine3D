#include "Precompiled.h"

#include "LanguageTypes/Constructor.h"

Constructor::Constructor(const Cursor &cursor, const Namespace &currentNamespace)
    : LanguageType( cursor, currentNamespace )
    , Invokable( cursor )
{
        
}