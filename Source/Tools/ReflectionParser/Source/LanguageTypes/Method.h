#pragma once

#include "LanguageType.h"

class Method : public LanguageType
{
public:
    Method(const Cursor &cursor, const Namespace &currentNamespace);
};