#pragma once

#include "LanguageType.h"
#include "Invokable.h"

class Class;

class Function 
    : public LanguageType
    , public Invokable
{
public:
    Function(const Cursor &cursor, const Namespace &currentNamespace, Class *parent = nullptr);

private:
    Class *m_parent;

    std::string m_name;
    std::string m_qualifiedName;
};