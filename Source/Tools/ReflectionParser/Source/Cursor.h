/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Cursor.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "CursorType.h"

class Cursor
{
public:
    typedef std::vector<Cursor> List;
    typedef CXCursorVisitor Visitor;

    Cursor(const CXCursor &handle);

    CXCursorKind GetKind(void) const;

    Cursor GetLexicalParent(void) const;
    Cursor GetTemplateSpecialization(void) const;

    std::string GetSpelling(void) const;
    std::string GetDisplayName(void) const;
    std::string GetMangledName(void) const;
    std::string GetUSR(void) const;

    std::string GetSourceFile(void) const;

    bool IsDefinition(void) const;
    bool IsConst(void) const;
    bool IsStatic(void) const;

    CX_CXXAccessSpecifier GetAccessModifier(void) const;
    CX_StorageClass GetStorageClass(void) const;

    CursorType GetType(void) const;
    CursorType GetReturnType(void) const;
    CursorType GetTypedefType(void) const;

    List GetChildren(void) const;
    void VisitChildren(Visitor visitor, void *data = nullptr);

    unsigned GetHash(void) const;

private:
    CXCursor m_handle;
};