#pragma once

#include "LanguageType.h"

namespace language_types
{
    class Class;

    class Global : public LanguageType
    {
    public:
        Global(const Cursor &cursor, const Namespace &currentNamespace, Class *parent = nullptr);

    private:
        Class *m_parent;
    };
}