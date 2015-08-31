#pragma once

#include "LanguageType.h"

namespace language_types
{
    class Field : public LanguageType
    {
    public:
        Field(const Cursor &cursor, const Namespace &currentNamespace);
    };
}