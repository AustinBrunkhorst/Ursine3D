/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Token.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

template<typename TokenEnumType, typename InputType>
struct Token
{
    typedef TokenEnumType EnumType;
    typedef InputType InputValueType;

    TokenEnumType type;
    InputType value;

    Token(void);
    Token(TokenEnumType type, const InputType &value);
};

#include "Impl/Token.hpp"