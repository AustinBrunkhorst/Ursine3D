/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Invokable.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Cursor.h"
#include "Namespace.h"

class Invokable
{
public:
    typedef std::vector<std::string> Signature;

    Invokable(const Cursor &cursor);

protected:
    std::string m_returnType;

    Signature m_signature;

    TemplateData compileSignatureTemplate(void) const;
};

const auto kReturnTypeVoid = "void";