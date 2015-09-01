#pragma once

#include "LanguageType.h"

class Class;

class Global : public LanguageType
{
public:
    Global(const Cursor &cursor, const Namespace &currentNamespace, Class *parent = nullptr);

private:
    bool m_isConst;

    Class *m_parent;

    std::string m_name;
    std::string m_displayName;
    std::string m_qualifiedName;
    std::string m_type;
};