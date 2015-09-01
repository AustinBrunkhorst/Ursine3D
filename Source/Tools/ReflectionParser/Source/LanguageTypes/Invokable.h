#pragma once

#include "Cursor.h"
#include "Namespace.h"

class Invokable
{
public:
    Invokable(const Cursor &cursor, const Namespace &currentNamespace);
};