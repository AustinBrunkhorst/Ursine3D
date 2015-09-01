#pragma once

#include "LanguageType.h"

class Constructor : public LanguageType
{
public:
    Constructor(const Cursor &cursor, const Namespace &currentNamespace);
};