/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TokenizerState.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <memory>
#include <unordered_map>

template<typename TokenEnumType, typename InputType>
class Tokenizer;

template<typename TokenEnumType, typename InputType>
class TokenizerState
{
public:
    typedef TokenizerState* Handle;

    explicit TokenizerState(TokenEnumType acceptingType);

    void SetAcceptingType(TokenEnumType acceptingType);

    void AddEdge(Handle to, char character);

    template<typename... CharacterList>
    void AddEdge(Handle to, char character, CharacterList &&...list);

    template<typename... CharacterList>
    void SetLooping(CharacterList &&...list);

    void SetDefaultEdge(Handle defaultEdge);

private:
    friend class Tokenizer<TokenEnumType, InputType>;

    TokenEnumType m_acceptingType;

    std::unordered_map<char, Handle> m_edges;
    Handle m_defaultEdge;
};

#include "Impl/TokenizerState.hpp"