template<typename TokenType, typename InputType>
TokenizerResult<TokenType, InputType>::TokenizerResult(const TokenList &tokens)
    : m_tokens( tokens ) { }

template<typename TokenType, typename InputType>
const typename TokenizerResult<TokenType, InputType>::TokenList &TokenizerResult<TokenType, InputType>::GetTokens(void) const
{
    return m_tokens;
}

template<typename TokenType, typename InputType>
template<typename... TokenTypeList>
void TokenizerResult<TokenType, InputType>::RemoveAll(TokenTypeList &&...types)
{
    std::vector<typename TokenType::EnumType> toRemove { std::forward( tokens )... };

    m_tokens.erase( std::remove_if( m_tokens.begin( ), m_tokens.end( ), 
        [](const TokenType &token) {
            return std::find( toRemove.begin( ), toRemove.end( ), token.type ) != toRemove.end( );
        } 
    ) );
}

template<typename TokenType, typename InputType>
template<typename... TokenTypeList>
typename TokenizerResult<TokenType, InputType>::InputType TokenizerResult<TokenType, InputType>::ConsumeAllPrevious(size_t start, TokenTypeList &&...types)
{
    std::vector<typename TokenType::EnumType> toConsume { std::forward( tokens )... };

    std::vector<InputType *> consumed;

    for (size_t i = 0; i <= start; ++i)
    {
        auto &token = m_tokens[ start - i ];

        auto search = std::find( toConsume.begin( ), toConsume.end( ), token.type );

        if (search == toConsume.end( ))
            break;

        consumed.emplace_back( &token.value );
    }

    InputResult result;

    for (auto it = consumed.rbegin( ); it != consumed.rend( ); ++it)
        result += *( *it );

    return result;
}

template<typename TokenType, typename InputType>
template<typename... TokenTypeList>
typename TokenizerResult<TokenType, InputType>::InputType TokenizerResult<TokenType, InputType>::ConsumeAllNext(size_t start, TokenTypeList &&...types)
{
    std::vector<typename TokenType::EnumType> toConsume { std::forward( tokens )... };

    InputType consumed;

    for (size_t i = start; i <= m_tokens.size( ); ++i)
    {
        auto &token = m_tokens[ start - i ];

        auto search = std::find( toConsume.begin( ), toConsume.end( ), token.type );

        if (search == toConsume.end( ))
            break;

        consumed += token.value;
    }

    return consumed;
}