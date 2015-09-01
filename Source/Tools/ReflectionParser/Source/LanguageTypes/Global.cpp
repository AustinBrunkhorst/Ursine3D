#include "Precompiled.h"

#include "LanguageTypes/Global.h"
#include "LanguageTypes/Class.h"

Global::Global(const Cursor &cursor, const Namespace &currentNamespace, Class *parent)
    : LanguageType( cursor, currentNamespace )
    , m_parent( parent )
{
        
}