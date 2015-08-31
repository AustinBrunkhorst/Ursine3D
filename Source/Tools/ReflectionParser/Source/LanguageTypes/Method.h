#pragma once

#include "LanguageType.h"

namespace language_types
{
    class Method : public LanguageType
    {
    public:
        Method(const Cursor &cursor, const Namespace &currentNamespace);
    };
}