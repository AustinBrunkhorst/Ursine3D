#pragma once

#include "LanguageType.h"

class Field : public LanguageType
{
public:
    Field(const Cursor &cursor, const Namespace &currentNamespace);
};