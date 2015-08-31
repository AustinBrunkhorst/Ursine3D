#pragma once

#include "LanguageType.h"

namespace language_types
{
    class Constructor : public LanguageType
    {
    public:
        Constructor(const Cursor &cursor, const Namespace &currentNamespace);
    };
}