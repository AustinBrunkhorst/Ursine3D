#pragma once

#include "LanguageType.h"

class Class;

class Field : public LanguageType
{
public:
    Field(const Cursor &cursor, const Namespace &currentNamespace, Class *parent = nullptr);
    virtual ~Field(void) { }

    TemplateData CompileTemplate(const ReflectionParser *context) const override;

private:
    bool m_isConst;

    bool m_hasExplicitGetter;
    bool m_hasExplicitSetter;

    Class *m_parent;

    std::string m_name;
    std::string m_displayName;
    std::string m_type;

    bool isAccessible(void) const;
    bool isGetterAccessible(void) const;
    bool isSetterAccessible(void) const;
};