/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Function.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "LanguageType.h"
#include "Invokable.h"

class Class;

class Function 
    : public LanguageType
    , public Invokable
{
public:
    Function(
        const Cursor &cursor, 
        const Namespace &currentNamespace, 
        Class *parent = nullptr
    );

    virtual ~Function(void) { }

    bool ShouldCompile(void) const;

    TemplateData CompileTemplate(
        const ReflectionParser *context
    ) const override;

private:
    Class *m_parent;

    std::string m_name;
    std::string m_qualifiedName;

    bool isAccessible(void) const;

    std::string getQualifiedSignature(void) const;
};