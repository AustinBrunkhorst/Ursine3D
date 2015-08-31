#pragma once

#include "LanguageType.h"

namespace language_types
{
    class Class;

    class Function : public LanguageType
    {
    public:
        Function(const Cursor &cursor, const Namespace &currentNamespace, Class *parent = nullptr);

    private:
        Class *m_parent;
    };
}