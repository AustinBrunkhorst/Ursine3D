/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TokenType.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#define TOKEN(name, value) name,

enum class ConstructorTokenType
{
    Invalid,
    Whitespace,
    Identifier,
    IntegerLiteral,
    FloatLiteral,
    StringLiteral,
    SYMBOL_START,

    #include "ConstructorTokenSymbols.inl"

    SYMBOL_END
};

#undef TOKEN