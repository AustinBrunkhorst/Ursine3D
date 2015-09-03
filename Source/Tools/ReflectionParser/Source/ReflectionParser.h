#pragma once

#include "ReflectionOptions.h"

#include "Cursor.h"
#include "Namespace.h"

#include "LanguageTypes/Class.h"
#include "LanguageTypes/Global.h"
#include "LanguageTypes/Function.h"
#include "LanguageTypes/Enum.h"

class ReflectionParser
{
public:
    ReflectionParser(const ReflectionOptions &options);
    ~ReflectionParser(void);

    void Parse(void);

    std::string GenerateHeader(const std::string &tmpl) const;
    std::string GenerateSource(const std::string &tmpl) const;

private:
    ReflectionOptions m_options;

    CXIndex m_index;
    CXTranslationUnit m_translationUnit;

    std::vector<Class*> m_classes;
    std::vector<Global*> m_globals;
    std::vector<Function*> m_globalFunctions;
    std::vector<Enum*> m_enums;

    void buildClasses(const Cursor &cursor, Namespace &currentNamespace);
    void buildGlobals(const Cursor &cursor, Namespace &currentNamespace);
    void buildGlobalFunctions(const Cursor &cursor, Namespace &currentNamespace);
    void buildEnums(const Cursor &cursor, Namespace &currentNamespace);

    TemplateData compileClassTemplates(void) const;
    TemplateData compileGlobalTemplates(void) const;
    TemplateData compileGlobalFunctionTemplates(void) const;
    TemplateData compileEnumTemplates(void) const;
};