#pragma once

#include "LanguageType.h"

class Field : public LanguageType
{
public:
    Field(const Cursor &cursor, const Namespace &currentNamespace);

private:
    std::string m_name;
    std::string m_displayName;
    std::string m_type;
};