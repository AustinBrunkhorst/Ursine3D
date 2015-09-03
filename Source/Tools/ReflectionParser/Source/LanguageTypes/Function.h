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
    virtual ~Function(void) { }

    TemplateData CompileTemplate(void) const override;

private:
    Class *m_parent;

    std::string m_name;
    std::string m_qualifiedName;

    bool isAccessible(void) const;

    std::string getQualifiedSignature(void) const;
    std::string getInvocationBody(void) const;
};