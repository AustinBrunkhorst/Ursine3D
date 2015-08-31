#pragma once

#include "Cursor.h"
#include "Namespace.h"

namespace language_types
{
    class Invokable
    {
    public:
        Invokable(const Cursor &cursor, const Namespace &currentNamespace);
    };
}
