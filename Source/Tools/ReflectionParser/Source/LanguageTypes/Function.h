#pragma once

#include "LanguageType.h"

class Class;

class Function : public LanguageType
{
public:
    Function(const Cursor &cursor, const Namespace &currentNamespace, Class *parent = nullptr);

private:
    Class *m_parent;
};