#pragma once

#include "LanguageType.h"

class Class;

class Global : public LanguageType
{
public:
    Global(const Cursor &cursor, const Namespace &currentNamespace, Class *parent = nullptr);
    virtual ~Global(void) { }

    TemplateData CompileTemplate(void) const override;

private:
    bool m_isConst;

    bool m_hasExplicitGetter;
    bool m_hasExplicitSetter;

    Class *m_parent;

    std::string m_name;
    std::string m_displayName;
    std::string m_qualifiedName;
    std::string m_type;

    static const TemplateData::PartialType m_getterPartial;
    static const TemplateData::PartialType m_setterPartial;

    bool isAccessible(void) const;
    bool isGetterAccessible(void) const;
    bool isSetterAccessible(void) const;

    std::string getGetterBody(void) const;
    std::string getSetterBody(void) const;
};