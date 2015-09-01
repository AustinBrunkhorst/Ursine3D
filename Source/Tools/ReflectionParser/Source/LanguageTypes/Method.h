#pragma once

#include "LanguageType.h"
#include "Invokable.h"

class Method 
    : public LanguageType
    , public Invokable
{
public:
    Method(const Cursor &cursor, const Namespace &currentNamespace);

private:
    bool m_isConst;

    std::string m_name;
};