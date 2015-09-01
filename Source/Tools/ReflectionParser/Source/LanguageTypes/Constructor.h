#pragma once

#include "LanguageType.h"
#include "Invokable.h"

class Constructor 
    : public LanguageType
    , public Invokable
{
public:
    Constructor(const Cursor &cursor, const Namespace &currentNamespace);
};