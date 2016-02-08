/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Field.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "LanguageType.h"

class Class;

class Field : public LanguageType
{
public:
    Field(
        const Cursor &cursor, 
        const Namespace &currentNamespace, 
        Class *parent = nullptr
    );

    virtual ~Field(void) { }

    bool ShouldCompile(void) const;
    
    TemplateData CompileTemplate(
        const ReflectionParser *context
    ) const override;

private:
    bool m_isConst;

    bool m_hasExplicitGetter;
    bool m_hasExplicitSetter;

    Class *m_parent;

    std::string m_name;
    std::string m_displayName;
    std::string m_type;

    std::string m_explicitGetter;
    std::string m_explicitSetter;

    std::string m_veryExplicitGetter;
    std::string m_veryExplicitSetter;

    bool isAccessible(void) const;
    bool isGetterAccessible(void) const;
    bool isSetterAccessible(void) const;
};