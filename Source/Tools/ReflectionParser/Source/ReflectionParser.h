/* ----------------------------------------------------------------------------
** Team Bear King
** © 201x DigiPen Institute of Technology, All Rights Reserved.
**
** ReflectionParser.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include "ReflectionOptions.h"

#include "Cursor.h"
#include "Namespace.h"

#include "Templates.h"

#include "ModuleFile.h"

class Class;
class External;
class Global;
class Function;
class Enum;

class ReflectionParser
{
public:
    ReflectionParser(const ReflectionOptions &options);
    ~ReflectionParser(void);

    void Parse(void);
    void GenerateFiles(void);

    MustacheTemplate LoadTemplate(const std::string &name) const;

    TemplateData::PartialType LoadTemplatePartial(
        const std::string &name
    ) const;

    void GenerateHeader(std::string &output) const;
    void GenerateSource(std::string &output) const;

private:
    ReflectionOptions m_options;

    CXIndex m_index;
    CXTranslationUnit m_translationUnit;

    MustacheTemplate m_moduleFileHeaderTemplate;
    MustacheTemplate m_moduleFileSourceTemplate;

    mutable std::unordered_map<
        std::string, 
        std::string
    > m_templatePartialCache;

    std::vector<std::shared_ptr<External>> m_externals;
    std::unordered_map<std::string, ModuleFile> m_moduleFiles;

    void buildClasses(
        const Cursor &cursor, 
        Namespace &currentNamespace
    );

    void buildGlobals(
        const Cursor &cursor, 
        Namespace &currentNamespace
    );

    void buildGlobalFunctions(
        const Cursor &cursor, 
        Namespace &currentNamespace
    );

    void buildEnums(
        const Cursor &cursor, 
        Namespace &currentNamespace
    );

    void addGlobalTemplateData(TemplateData &data);

    void generateModuleFile(
        const fs::path &fileHeader, 
        const fs::path &fileSource, 
        const ModuleFile &file
    );
};