/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TokenizerResult.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

template<typename TokenType>
class TokenizerResult
{
public:
    typedef std::vector<TokenType> TokenList;

    TokenizerResult(const TokenList &tokens);

    const TokenList &GetTokens(void) const;

    template<typename... TokenTypeList>
    size_t FindNext(size_t start, TokenTypeList &&...types);

    template<typename... TokenTypeList>
    size_t FindPrevious(size_t start, TokenTypeList &&...types);

    template<typename... TokenTypeList>
    void RemoveAll(TokenTypeList &&...types);

    typename TokenType::InputValueType ConsumeRange(size_t start, size_t end);

    template<typename... TokenTypeList>
    typename TokenType::InputValueType ConsumeAllPrevious(size_t start, TokenTypeList &&...types);

    template<typename... TokenTypeList>
    typename TokenType::InputValueType ConsumeAllNext(size_t start, TokenTypeList &&...types);
private:
    TokenList m_tokens;
};

#include "Impl/TokenizerResult.hpp"