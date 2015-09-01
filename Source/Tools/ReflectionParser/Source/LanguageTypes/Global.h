#pragma once

#include "LanguageType.h"

class Class;

class Global : public LanguageType
{
public:
    Global(const Cursor &cursor, const Namespace &currentNamespace, Class *parent = nullptr);

private:
    Class *m_parent;
};